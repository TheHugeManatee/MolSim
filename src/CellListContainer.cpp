/*
 * @file CellListContainer.cpp
 *
 * @date 20.11.2012
 * @author Jakob Weiss
 */

#include "CellListContainer.h"

#include "utils/Settings.h"

#include <cassert>
#include <algorithm>
#include<float.h>


log4cxx::LoggerPtr CellListContainer::logger = log4cxx::Logger::getLogger("CellListContainer");
long long CellListContainer::cellSwitches = 0;


CellListContainer::CellListContainer() {
	cellSwitches = 0;
	edgeLength = Settings::rCutoff;
	std::cout <<"CutOff:" << edgeLength;
	//calculate the cells in each direction
	//add 2 for halo and another 2 for dead layer
	nX0 = std::ceil(Settings::domainSize[0] / edgeLength) + 4;
	nX1 = std::ceil(Settings::domainSize[1] / edgeLength) + 4;
	nX2 = std::ceil(Settings::domainSize[2] / edgeLength) + 4;

	size_ = 0;

	LOG4CXX_DEBUG(logger, "Simulation domain will be divided into " << nX0 << "x" << nX1 << "x" << nX2 << " cells");

	//create all the cells
	cells.resize(nX0*nX1*nX2);

}

CellListContainer::~CellListContainer() {
}


inline ParticleContainer * CellListContainer::getCell(int x0, int x1, int x2) {

	assert(x0 >= 1);
	assert(x0 < nX0-1);
	assert(x1 >= 1);
	assert(x1 < nX1-1);
	assert(x2 >= 1);
	assert(x2 < nX2-1);


	return &cells[x2 + x1*nX2 + x0*nX2*nX1];
};

ParticleContainer * CellListContainer::getContainingCell(Particle& p) {

	int x0 = (p.x[0]) / edgeLength + 2,
		x1 = (p.x[1]) / edgeLength + 2,
		x2 = (p.x[2]) / edgeLength + 2;

	//crop the indices to the halo layer
	if(p.x[0] > Settings::domainSize[0])
		x0 = nX0 - 2;
	else x0 = std::min(nX0 - 2, std::max(1, x0));

	if(p.x[1] > Settings::domainSize[1])
		x1 = nX1 - 2;
	else x1 = std::min(nX1 - 2, std::max(1, x1));

	if(p.x[2] > Settings::domainSize[2])
		x2 = nX2 - 2;
	else x2 = std::min(nX2 - 2, std::max(1, x2));

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

#ifdef FOR_PARALLEL
#pragma omp parallel for
#endif
	for(int x0=1; x0 < nX0-1; x0++)
		for(int x1=1; x1 < nX1-1; x1++)
			for(int x2=1; x2 < nX2-1; x2++) {
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
#include <omp.h>
#include <stdio.h>
void CellListContainer::eachPair(std::function<void (Particle &, Particle&)> fn) {
	double rcSquared = Settings::rCutoff*Settings::rCutoff;
#ifdef FOR_PARALLEL
#pragma omp parallel for
#endif
	for(int x0=1; x0 < nX0-1; x0++) {
		//printf("%i %i\n", tid, x0);
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
		std::function<bool (ParticleContainer &container, Particle &)> boundaryHandlers[6],
		std::function<bool (ParticleContainer &container, Particle &)> haloHandler) {
	int cellcount = cells.size();

	for(int x0=1; x0 < nX0-1; x0++)
		for(int x1=1; x1 < nX1-1; x1++)
			for(int x2=1; x2 < nX2-1; x2++) {
				ParticleContainer &c = cells[x2 + x1*nX2 + x0*nX2*nX1];
				int cellParticleCount = c.particles.size();
				for(int i = 0; i < cellParticleCount; i++) {
					Particle &p = c.particles[i];


					bool particleToBeRemoved = false;

					//if the halo handler or the boundary handler say the particle should be removed, kill it

					if(isHaloCell(x0, x1, x2)) {
						particleToBeRemoved = haloHandler(*this, p);
					}

					//Check for all boundaries
					else {
						if(x0 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[0](*this, p);
						if(x0 == (nX0 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[1](*this, p);
						if(x1 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[2](*this, p);
						if(x1 == (nX1 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[3](*this, p);
						if(x2 == 2) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[4](*this, p);
						if(x2 == (nX2 - 3)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[5](*this, p);
					}

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
	//LOG4CXX_TRACE(logger, "Cell switches: " << cellSwitches);
	//LOG4CXX_TRACE(logger, "Particles Left: " << getSize());
	//LOG4CXX_TRACE(logger, "Empty cells: " << emptyCells);
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

inline int CellListContainer::getSize() {
	int cellcount = cells.size();
	int size = 0;
	for(int ci = 0; ci < cellcount; ci++) {
		size += cells[ci].getSize();
	}
//	assert(size == size_);
	return size;
}


