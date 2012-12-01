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


log4cxx::LoggerPtr CellListContainer::logger = log4cxx::Logger::getLogger("CellListContainer");
long long CellListContainer::cellSwitches = 0;

CellListContainer::CellListContainer() {
	edgeLength = Settings::rCutoff;

	//calculate the cells in each direction
	//add 2 for halo
	nX0 = std::ceil(Settings::domainSize[0] / edgeLength) + 2;
	nX1 = std::ceil(Settings::domainSize[1] / edgeLength) + 2;
	nX2 = std::ceil(Settings::domainSize[2] / edgeLength) + 2;

	size_ = 0;

	LOG4CXX_DEBUG(logger, "Simulation domain will be divided into " << nX0 << "x" << nX1 << "x" << nX2 << " cells");

	//create all the cells
	cells.resize(nX0*nX1*nX2);
}

CellListContainer::~CellListContainer() {
}

inline ParticleContainer * CellListContainer::getCell(int x0, int x1, int x2) {

	assert(x0 >= 0);
	assert(x0 < nX0);
	assert(x1 >= 0);
	assert(x1 < nX1);
	assert(x2 >= 0);
	assert(x2 < nX2);

/*	if(	x0 < 0 || x0 > nX0 ||
		x1 < 0 || x1 > nX1 ||
		x2 < 0 || x2 > nX2)
		return NULL;*/

	return &cells[x2 + x1*nX2 + x0*nX2*nX1];
};

ParticleContainer * CellListContainer::getContainingCell(Particle& p) {
	int x0 = (p.x[0]) / edgeLength + 1,
		x1 = (p.x[1]) / edgeLength + 1,
		x2 = (p.x[2]) / edgeLength + 1;

	//LOG4CXX_TRACE(logger, "Cell position for particle " << p.x.toString() <<": " << x0 << " " << x1 << " " << x2);

	//crop the indices to the halo layer
	x0 = std::min(nX0 - 1, std::max(0, x0));
	x1 = std::min(nX1 - 1, std::max(0, x1));
	x2 = std::min(nX2 - 1, std::max(0, x2));

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
	for(int i = 0; i < s; i++) {
		cells[i].each(fn);
	}
}

void CellListContainer::eachPair(std::function<void (Particle &, Particle&)> fn) {
	for(int x0=0; x0 < nX0; x0++) {
		for(int x1=0; x1 < nX1; x1++) {
			for(int x2=0; x2 < nX2; x2++) {
				ParticleContainer &c = cells[x2 + x1*nX2 + x0*nX2*nX1];
				//process the appropriate adjacent cells
				//this will be 13 cell comparisons
				//we rely on compiler optimization on this one :)
				if(!c.getSize()) continue;
				c.eachPair(fn);

				/*(0,1,0)*/if(x1 + 1 < nX1)	eachPair(fn, c, cells[x2 + (x1+1)*nX2 + x0*nX2*nX1]);
				if(x2 + 1 < nX2) {
					/*(1,0,0)*/eachPair(fn, c, cells[x2 + 1 + x1*nX2 + x0*nX2*nX1]);
					if(x1 + 1 < nX1)
						/*(1,1,0)*/eachPair(fn, c, cells[x2 + 1 + (x1+1)*nX2 + x0*nX2*nX1]);
					if(x1 > 0)
						/*(1,-1,0)*/eachPair(fn, c, cells[x2 + 1 + (x1-1)*nX2 + x0*nX2*nX1]);
				}
				if(x0 > 0) {
					/*(0,0,-1)*/eachPair(fn, c, cells[x2 + x1*nX2 + (x0-1)*nX2*nX1]);

					/*(0,1,-1)*/if(x1 + 1 < nX1)	eachPair(fn, c, cells[x2 + (x1+1)*nX2 + (x0-1)*nX2*nX1]);
					if(x2 + 1 < nX2) {
						/*(1,0,-1)*/eachPair(fn, c, cells[x2 + 1 + x1*nX2 + (x0-1)*nX2*nX1]);
						if(x1 + 1 < nX1)
							/*(1,1,-1)*/eachPair(fn, c, cells[x2 + 1 + (x1+1)*nX2 + (x0-1)*nX2*nX1]);
						if(x1 > 0)
							/*(1,-1,-1)*/eachPair(fn, c, cells[x2 + 1 + (x1-1)*nX2 + (x0-1)*nX2*nX1]);
					}
				}
				if(x0 + 1 < nX0) {
					/*(0,1,1)*/if(x1 + 1 < nX1)	eachPair(fn, c, cells[x2 + (x1+1)*nX2 + (x0+1)*nX2*nX1]);
					if(x2 + 1 < nX2) {
						/*(1,0,1)*/eachPair(fn, c, cells[x2 + 1 + x1*nX2 + (x0+1)*nX2*nX1]);
						if(x1 + 1 < nX1)
							/*(1,1,1)*/eachPair(fn, c, cells[x2 + 1 + (x1+1)*nX2 + (x0+1)*nX2*nX1]);
						if(x1 > 0)
							/*(1,-1,1)*/eachPair(fn, c, cells[x2 + 1 + (x1-1)*nX2 + (x0+1)*nX2*nX1]);
					}
				}
			}
		}
	}
}

void CellListContainer::afterPositionChanges(
		std::function<bool (ParticleContainer &container, Particle &)> boundaryHandlers[6],
		std::function<bool (ParticleContainer &container, Particle &)> haloHandler) {
	int cellcount = cells.size();
	int emptyCells = 0;
	for(int x0=0; x0 < nX0; x0++)
		for(int x1=0; x1 < nX1; x1++)
			for(int x2=0; x2 < nX2; x2++) {
				ParticleContainer &c = cells[x2 + x1*nX2 + x0*nX2*nX1];
				int cellParticleCount = c.particles.size();
				for(int i = 0; i < cellParticleCount; i++) {
					Particle &p = c.particles[i];

					//reset force accumulator
					p.old_f = p.f;
					p.f = 0;

					bool particleToBeRemoved = false;

					//if the halo handler or the boundary handler say the particle should be removed, kill it

					if(isHaloCell(x0, x1, x2)) {
						particleToBeRemoved = haloHandler(*this, p);
					}
					//Check for all boundaries
					else {
						if(x0 == 1) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[0](*this, p);
						if(x0 == (nX0 - 2)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[1](*this, p);
						if(x1 == 1) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[2](*this, p);
						if(x1 == (nX1 - 2)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[3](*this, p);
						if(x2 == 1) 			particleToBeRemoved = particleToBeRemoved || boundaryHandlers[4](*this, p);
						if(x2 == (nX2 - 2)) 	particleToBeRemoved = particleToBeRemoved || boundaryHandlers[5](*this, p);
					}

					//if particle is not deleted, check if it should be in some different cell than it is now
					if(!particleToBeRemoved) {
						ParticleContainer *cc = getContainingCell(p);
						if(cc != &c) {
							cellSwitches++;

							assert(cc != NULL);

							cc->add(p);
							particleToBeRemoved = true;
						}
					}
					//Perform the removal
					if(particleToBeRemoved) {
						//switch last one and the one to be deleted
						c.particles[i] = c.particles[c.particles.size() - 1];
						//then delete the new last one and decrement the loop counter to check
						//the one at position i again
						c.particles.pop_back();
						cellParticleCount--;
						i--;
						size_--;
					}
#ifndef NDEBUG
					//some color coding for the cells for debugging
					else {
						int _x0 = (p.x[0]) / edgeLength + 1,
							_x1 = (p.x[1]) / edgeLength + 1,
							_x2 = (p.x[2]) / edgeLength + 1;
						p.type = (_x2 + _x1 + _x0) % 10;
						//p.type = (_x2 + _x1*nX2 + _x0*nX2*nX1) % 9;
					}
#endif
				}
				if(!cellParticleCount) emptyCells++;
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
			fn(c1.particles[i], c2.particles[j]);
		}
	}
	/*c1.each([&] (Particle &particle1) {
		c2.each([&] (Particle &particle2) {
			fn(particle1, particle2);
		});
	});*/
}

int CellListContainer::getSize() {
	int cellcount = cells.size();
	int size = 0;
	for(int ci = 0; ci < cellcount; ci++) {
		size += cells[ci].getSize();
	}
//	assert(size == size_);
	return size;
}
