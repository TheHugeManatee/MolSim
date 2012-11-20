/*
 * CellListContainer.h
 *
 *  Created on: 20.11.2012
 *      Author: j
 */

#ifndef CELLLISTCONTAINER_H_
#define CELLLISTCONTAINER_H_

#include "ParticleContainer.h"

#include "utils/Vector.h"

#include <log4cxx/logger.h>

#include <functional>


class CellListContainer: public ParticleContainer {
	friend class CellListContainerTest;

private:
	static log4cxx::LoggerPtr logger;

	/**
	 * the internal cells making up the simulation region
	 */
	std::vector<ParticleContainer> cells;

	/**
	 * number of cells in each direction
	 */
	int nX0, nX1, nX2;

	/**
	 * the edge length of each cell
	 */
	double edgeLength;

	ParticleContainer * getCell(int x0, int x1, int x2);

	ParticleContainer * getContainingCell(Particle& p);


public:
	CellListContainer();

	virtual ~CellListContainer();

	void updateCells();

	//Overridden members from ParticleContainer base class
    void each(std::function<void (Particle&)> fn);
    void eachPair(std::function<void (Particle&, Particle&)> fn);
    void add(Particle& p);
    int getSize();
};

#endif /* CELLLISTCONTAINER_H_ */
