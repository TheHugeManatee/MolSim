/*
 *  @file CellListContainer.h
 *
 *  @date 20.11.2012
 *  @author Jakob Weiss
 */

#ifndef CELLLISTCONTAINER_H_
#define CELLLISTCONTAINER_H_

#include "ParticleContainer.h"

#include "utils/Vector.h"

#include <log4cxx/logger.h>

#include <functional>

/**
 * @class CellListContainer
 *
 * This class implements a particle container that manages its particles by putting them into a regular grid of cells, each cell
 * being a standard particle container. The cell size depends on the Settings::rCutoff parameter, which defines the minimum
 * distance where forces will be neglected between particles.
 *
 * This approximation reduces the complexity from O(n^2) to O(n) because only particles in neighboring cells will be compared and computed
 *
 *
 */
class CellListContainer: public ParticleContainer {
	friend class CellListContainerTest;

private:
	static log4cxx::LoggerPtr logger;

	/**
	 * internal size counter. gets incremented on every add()
	 */
	int size_;

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

	/**
	 * returns the cell indexed by the three cell coordinates
	 */
	inline ParticleContainer * getCell(int x0, int x1, int x2);

	/**
	 * returns the cell the particle should be in right now,
	 * based on the current position
	 */
	inline ParticleContainer * getContainingCell(Particle& p);

	/**
	 * applies a function to pairs of particles combined from two containers
	 * particle pairs from within one container will NOT be processed
	 * essentially this can be seen as an operation on the cross product of the two sets of particles
	 */
	inline void eachPair(std::function<void (Particle&, Particle&)> fn, ParticleContainer &p1, ParticleContainer &p2);

	inline bool isHaloCell(int x0, int x1, int x2) {
		return (!x0 || !x1 || !x2 || (x0 == (nX0-1)) || (x1 == (nX1-1)) || (x2 == (nX2-1)));
	}

	inline bool isBoundaryCell(int x0, int x1, int x2) {
		return (x0 == 1 || x1 == 1 || x2 == 1 ||
				x0 == nX0 - 2 ||
				x1 == nX1 - 2 ||
				x2 == nX2 - 2);

	}

public:
	static long long cellSwitches;

	CellListContainer();

	virtual ~CellListContainer();

	void afterPositionChanges(
			std::function<bool (Particle &, utils::Vector<double, 3> &)> boundaryHandler,
			std::function<bool (Particle &, utils::Vector<double, 3> &)> haloHandler
	);

	//Overridden members from ParticleContainer base class
    void each(std::function<void (Particle&)> fn);

    /**
     * eachPair method overridden from ParticleContainer
     * this will ignore particle whose distances are greater than rCutoff to optimize performance
     * also, this will ignore particles in the halo layer of the cell list
     * @override
     */
    void eachPair(std::function<void (Particle&, Particle&)> fn);
    void add(Particle& p);
    int getSize();
};

#endif /* CELLLISTCONTAINER_H_ */
