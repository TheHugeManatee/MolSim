/*
 * CellListContainer.cpp
 *
 *  Created on: 20.11.2012
 *      Author: j
 */

#include "CellListContainer.h"

#include "utils/Settings.h"

#include <cassert>
#include <algorithm>

log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("CellListContainer");

CellListContainer::CellListContainer() {
	edgeLength = Settings::rCutoff;

	nX0 = std::ceil(Settings::domainSize[0] / edgeLength);
	nX1 = std::ceil(Settings::domainSize[1] / edgeLength);
	nX2 = std::ceil(Settings::domainSize[2] / edgeLength);

	LOG4CXX_DEBUG(logger, "Simulation domain will be divided into " << nX0 << "x" << nX1 << "x" << nX2 << " cells");

	cells.resize(nX0*nX1*nX2);

	//create all the cells
	for(int x0=0; x0 < nX0; x0++)
		for(int x1=0; x1 < nX1; x1++)
			for(int x2=0; x2 < nX2; x2++)
				cells.push_back(ParticleContainer());

}

CellListContainer::~CellListContainer() {
	// TODO Auto-generated destructor stub
}

inline ParticleContainer & CellListContainer::getCell(int x0, int x1, int x2) {
	assert(x0 > 0);
	assert(x1 > 0);
	assert(x2 > 0);
	assert(x0 < nX0);
	assert(x1 < nX1);
	assert(x2 < nX2);

	return cells[x0 + x1*nX1 + x2*nX1*nX2];
};

ParticleContainer & CellListContainer::getContainingCell(Particle& p) {
	int x0 = p.x[0] / edgeLength,
		x1 = p.x[1] / edgeLength,
		x2 = p.x[2] / edgeLength;

	return getCell(x0, x1, x2);
}

void CellListContainer::add(Particle & p) {
	ParticleContainer & cell = getContainingCell(p);
}
