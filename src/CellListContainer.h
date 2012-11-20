/*
 * CellListContainer.h
 *
 *  Created on: 20.11.2012
 *      Author: j
 */

#ifndef CELLLISTCONTAINER_H_
#define CELLLISTCONTAINER_H_

#include <functional>

#include "utils/Vector.h"

#include "ParticleContainer.h"

class CellListContainer: public ParticleContainer {
private:
	static log4cxx::LoggerPtr logger;

	/**
	 * the internal cells making up the simulation region
	 */
	std::vector<ParticleContainer> cells;

	/**
	 * number of cells in each direction
	 */
	int nX1, nX2, nX3;

public:
	CellListContainer();

	virtual ~CellListContainer();

	void updateCells();
};

#endif /* CELLLISTCONTAINER_H_ */
