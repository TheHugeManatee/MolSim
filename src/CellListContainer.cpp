/*
 * @file CellListContainer.cpp
 *
 * @date 20.11.2012
 * @author Jakob Weiss
 */

#include "CellListContainer.h"

#include "utils/Settings.h"
#include "JobQueue.h"

#include <cassert>
#include <algorithm>
#include<float.h>


log4cxx::LoggerPtr CellListContainer::logger = log4cxx::Logger::getLogger("CellListContainer");
long long CellListContainer::cellSwitches = 0;


CellListContainer::CellListContainer() {
	cellSwitches = 0;

	//calculate the cells in each direction
	//add 2 for halo and another 2 for dead layer
	nX0 = std::ceil(Settings::domainSize[0] / Settings::rCutoff) + 4;
	nX1 = std::ceil(Settings::domainSize[1] / Settings::rCutoff) + 4;
	nX2 = std::ceil(Settings::domainSize[2] / Settings::rCutoff) + 4;

	edgeLength[0] = Settings::domainSize[0] / (nX0-4);
	edgeLength[1] = Settings::domainSize[1] / (nX1-4);
	edgeLength[2] = Settings::domainSize[2] / (nX2-4);

	size_ = 0;

	LOG4CXX_DEBUG(logger, "Simulation domain will be divided into " << nX0 << "x" << nX1 << "x" << nX2 << " cells");
	LOG4CXX_DEBUG(logger, "cell sizes are " << edgeLength[0] << "x" << edgeLength[1] << "x" << edgeLength[2] << " cells");


	//create all the cells
	cells.resize(nX0*nX1*nX2);

	haloCells.reserve(2*((nX0-2)*(nX1-2) + (nX0-2)*(nX2-2) + (nX1-2)*(nX2-2)));

	std::cout << "Halo cells size: " << haloCells.size();

	for(int x0=1; x0 < nX0-1; x0++)
		for(int x1=1; x1 < nX1-1; x1++)
			for(int x2=1; x2 < nX2-1; x2++) {
				if(isHaloCell(x0, x1, x2)) {
					haloCells.push_back(&cells[x2 + x1*nX2 + x0*nX2*nX1]);
				}
			}
}

CellListContainer::~CellListContainer() {
}


ParticleContainer * CellListContainer::getCell(int x0, int x1, int x2) {

	//std::cout << "retrieving " << x0 << " , " << x2 << " , " << x2 << std::endl;
	assert(x0 >= 1);
	assert(x0 < nX0-1);
	assert(x1 >= 1);
	assert(x1 < nX1-1);
	assert(x2 >= 1);
	assert(x2 < nX2-1);


	return &cells[x2 + x1*nX2 + x0*nX2*nX1];
};

ParticleContainer * CellListContainer::getContainingCell(Particle& p) {

	int x0 = (p.x[0]) / edgeLength[0] + 2,
		x1 = (p.x[1]) / edgeLength[1] + 2,
		x2 = (p.x[2]) / edgeLength[2] + 2;

	//crop the indices to the halo layer
	x0 = std::min(nX0 - 2, std::max(1, x0));
	x1 = std::min(nX1 - 2, std::max(1, x1));
	x2 = std::min(nX2 - 2, std::max(1, x2));

//	std::cout << "Added particle at " << p.x << " to Layer: " << x0 <<" " << x1 << " "<< x2  << std::endl;

	return getCell(x0, x1, x2);
}

void CellListContainer::add(Particle & p) {

	ParticleContainer *cell = getContainingCell(p);

	assert(cell != NULL);
	cell->add(p);
	size_++;
}

void CellListContainer::each(std::function<void (Particle &)> fn) {
	int s = cells.size();
#ifdef _OPENMP
#ifdef FOR_PARALLEL
#pragma omp parallel for
#endif
#endif
	for(int x0=2; x0 < nX0-2; x0++)
		for(int x1=2; x1 < nX1-2; x1++)
			for(int x2=2; x2 < nX2-2; x2++) {
				std::vector<Particle> &plist = cells[x2 + x1*nX2 + x0*nX2*nX1].particles;
				int s = plist.size();
				for(int i=0; i < s; i++)
					fn(plist[i]);
			}

}

/**
 * this macro loops over all pairs of particles in the two containers and applies fn to the pair of them
 * this is slightly faster than using an inline function
 * it assumes the variables sc1 and rcSquared are already defined.
 */
#define EACHPAIR(fn, c1, c2) {\
	int sc2 = c2.getSize();	\
	for(int j = 0; j < sc2; j++) { \
		for(int i = 0; i < sc1; i++) { \
			if((c1.particles[i].x -c2.particles[j].x).LengthOptimizedR3Squared()<rcSquared) \
		fn(c1.particles[i], c2.particles[j]); \
		}\
	}\
}

void CellListContainer::eachPair(std::function<void (Particle &, Particle&)> fn) {
	double rcSquared = Settings::rCutoff*Settings::rCutoff;
#ifdef _OPENMP
#ifdef FOR_PARALLEL
#pragma omp parallel for
#endif
#endif
	for(int x0=1; x0 < nX0-1; x0++) {
		for(int x1=1; x1 < nX1-1; x1++) {
			for(int x2=1; x2 < nX2-1; x2++) {
				int cid = x2 + x1*nX2 + x0*nX2*nX1;
				ParticleContainer &c = cells[cid];
				int sc1 = c.getSize();

				//process the appropriate adjacent cells
				//this will be 13 cell comparisons
				//we rely on compiler optimization on this one :)
				if(!sc1) continue;
				c.eachPair(fn);


				/*(0,1,0)*/	EACHPAIR(fn, c, cells[cid + nX2]);
				/*(1,0,0)*/EACHPAIR(fn, c, cells[cid + 1]);
				/*(1,1,0)*/EACHPAIR(fn, c, cells[cid + 1 + nX2]);
				/*(1,-1,0)*/EACHPAIR(fn, c, cells[cid + 1 - nX2]);
				/*(0,0,-1)*/EACHPAIR(fn, c, cells[cid - nX2*nX1]);

				/*(0,1,-1)*/EACHPAIR(fn, c, cells[cid + nX2 - nX2*nX1]);
				/*(1,0,-1)*/EACHPAIR(fn, c, cells[cid + 1 - nX2*nX1]);
				/*(1,1,-1)*/EACHPAIR(fn, c, cells[cid + 1 + nX2 - nX2*nX1]);
				/*(1,-1,-1)*/EACHPAIR(fn, c, cells[cid + 1 - nX2 - nX2*nX1]);
				/*(0,1,1)*/EACHPAIR(fn, c, cells[cid + nX2 + nX2*nX1]);
				/*(1,0,1)*/EACHPAIR(fn, c, cells[cid + 1 + nX2*nX1]);
				/*(1,1,1)*/EACHPAIR(fn, c, cells[cid + 1 + nX2 + nX2*nX1]);
				/*(1,-1,1)*/EACHPAIR(fn, c, cells[cid + 1 - nX2 + nX2*nX1]);
			}
		}
	}
}

void CellListContainer::afterPositionChanges(
		std::function<bool (ParticleContainer &container, Particle &)> boundaryHandlers[6]) {
	int cellcount = cells.size();
#ifdef _OPENMP
//#pragma omp parallel for
#endif
	for(int x0=2; x0 < nX0-2; x0++)
		for(int x1=2; x1 < nX1-2; x1++)
			for(int x2=2; x2 < nX2-2; x2++) {
				ParticleContainer &c = cells[x2 + x1*nX2 + x0*nX2*nX1];
				int cellParticleCount = c.particles.size();
				for(int i = 0; i < cellParticleCount; i++) {
					Particle &p = c.particles[i];


					bool particleToBeRemoved = false;

					//Check for all boundaries
					if(x0 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[0](*this, p);
					if(x0 == (nX0 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[1](*this, p);
					if(x1 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[2](*this, p);
					if(x1 == (nX1 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[3](*this, p);
					if(x2 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[4](*this, p);
					if(x2 == (nX2 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[5](*this, p);

					//if particle is not deleted, check if it should be in some different cell than it is now
					if(!particleToBeRemoved) {
						ParticleContainer *cc = getContainingCell(p);
						if(cc != &c) {
#ifndef NDEBUG
							cellSwitches++;
							assert(cc != NULL);
#endif
							cc->add(p);
							particleToBeRemoved = true;
						}
					}
					//Perform the removal
					if(particleToBeRemoved) {
						//switch last one and the one to be delete
						c.particles[i] = c.particles[c.particles.size() - 1];
						//then delete the new last one and decrement the loop counter to check
						//the one at position i again
						c.particles.pop_back();
						cellParticleCount--;
						i--;
						size_--;
					}
				}
	}

}


void CellListContainer::eachPair(std::function<void (Particle&, Particle&)> fn, ParticleContainer &c1, ParticleContainer &c2) {
	int sc1 = c1.getSize(),
		sc2 = c2.getSize();

	//direct looping should achieve better performance than nested lambdas
	for(int i = 0; i < sc1; i++) {
		for(int j = 0; j < sc2; j++) {
			if((c1.particles[i].x -c2.particles[j].x).LengthOptimizedR3Squared()<Settings::rCutoff*Settings::rCutoff)
				fn(c1.particles[i], c2.particles[j]);
		}
	}
}

int CellListContainer::getSize(bool withHalo) {
	int cellcount = cells.size();
	int size = 0;
	int k = withHalo?1:2;
	for(int x0=k; x0 < nX0-k; x0++)
		for(int x1=k; x1 < nX1-k; x1++)
			for(int x2=k; x2 < nX2-k; x2++) {
				std::vector<Particle> &plist = cells[x2 + x1*nX2 + x0*nX2*nX1].particles;
				size += plist.size();
	}

	return size;
}

void CellListContainer::clearHalo() {
	int s = haloCells.size();
	for(int i=0; i < s; i++) {
		haloCells[i]->particles.clear();
	}
}
