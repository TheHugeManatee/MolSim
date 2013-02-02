/*
 *  @file CellListContainer.h
 *
 *  @date 20.11.2012
 *  @author Jakob Weiss
 */

#ifndef CELLLISTCONTAINER_H_
#define CELLLISTCONTAINER_H_

#include "utils/Settings.h"
#include "ParticleContainer.h"
#include "utils/Vector.h"

#include <log4cxx/logger.h>

#include <functional>

namespace outputWriter {
		class RenderOutputWriter;
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

/**
 * @class CellListContainer
 *
 * This class implements a particle container that manages its particles by putting them into a regular grid of cells, each cell
 * being a standard particle container. The cell size depends on the Settings::rCutoff parameter, which defines the minimum
 * distance where forces will be neglected between particles.
 *
 * This approximation reduces the complexity compared to the naive ParticleContainer of the eachPairs method from O(n^2) to O(n)
 * because the function will only be executed for pairs of particles which lie in adjacent cells
 *
 * The graph illustrates the benefit the CelllistContainer yields compared to the naive implementation in a simulation of 10 seconds
 *
 * \image html https://dl.dropbox.com/u/16135288/MolSim/ComputationTimeCelllistcontainer.png
 *
 *
 */
class CellListContainer: public ParticleContainer {
	friend class CellListContainerTest; //the test needs to have access to the internal structures

	//these need to have access to achieve proper workload distribution
	friend class Job;
	friend class SimulationJobQueue;
	friend class APCJobQueue;
	friend class SliceJob;
	friend class BlockJob;
	friend class APCSliceJob;

	//this is needed to display internal structures of the container
	friend class outputWriter::RenderOutputWriter;

private:
	static log4cxx::LoggerPtr logger;
	std::vector<ParticleContainer *> haloCells;

protected:
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
	double edgeLength[3];

	/**
	 * returns the cell indexed by the three cell coordinates
	 */
	ParticleContainer * getCell(int x0, int x1, int x2);

	/**
	 * returns the cell the particle should be in right now,
	 * based on the current position
	 */
	ParticleContainer * getContainingCell(Particle& p);

	/**
	 * applies a function to pairs of particles combined from two containers
	 * particle pairs from within one container will NOT be processed
	 * essentially this can be seen as an operation on the cross product of the two sets of particles
	 */
	inline void eachPair(std::function<void (Particle&, Particle&)> fn, ParticleContainer &p1, ParticleContainer &p2);

	/**
	 * checks if a cell is in the halo
	 * @param x0 cell index in x0 direction
	 * @param x1 cell index in x1 direction
	 * @param x2 cell index in x2 direction
	 */
	inline bool isHaloCell(int x0, int x1, int x2) {
		if(Settings::dimensions == 3){
		return (x0==1 || x1==1 || x2==1 || (x0 == (nX0-2)) || (x1 == (nX1-2)) || (x2 == (nX2-2)));
		}
		return (x0==1 || x1==1 || (x0 == (nX0-2)) || (x1 == (nX1-2)));
	}

	/**
	 * checks if the cell is a boundary cell
	 * @param x0 cell index in x0 direction
	 * @param x1 cell index in x1 direction
	 * @param x2 cell index in x2 direction
	 */
	inline bool isBoundaryCell(int x0, int x1, int x2) {
		return (x0 == 2 || x1 == 2 || x2 == 2 ||
				x0 == (nX0 - 3) ||
				x1 == (nX1 - 3) ||
				x2 == (nX2 - 3) );

	}

public:
	static long long cellSwitches;

	CellListContainer();

	virtual ~CellListContainer();


	/**
	 * this is called by the simulator after all positions have been updated for the
	 * particles. This function has multiple purposes:
	 *  - copy force vector of each particle onto old_f and reset f to zero
	 *  - optimize the internal structure of the container regarding the updated positions
	 *  - apply boundary conditions
	 *
	 *  @param boundaryHandlers the handler function that will be called when a particle is
	 *  	detected to be near a boundary. This MAY be called for all particles, but "smart"
	 *  	containers will sort out particles in the inner areas of the domain
	 *  	- param ParticleContainer &container: the container instance we are working on
	 *  	- param Particle &p: reference to the particle in question
	 *  	- return bool: whether the particle should be removed (deleted) from the container
	 *
	 *  @warning The boundaryHandler and haloHandler functionals may be called more than once for
	 *  	one unique particle due to the internal restructuring of the particle container
	 *  	so make sure this will not be a problem
	 */
	void afterPositionChanges(
			std::function<bool (ParticleContainer &container, Particle &)> boundaryHandlers[6]);

    void each(std::function<void (Particle&)> fn);

    /**
     * eachPair method overridden from ParticleContainer
     * this will ignore particle whose distances are greater than rCutoff to optimize performance
     * also, this will ignore particles in the halo layer of the cell list
     * @override
     */
    void eachPair(std::function<void (Particle&, Particle&)> fn);

    /**
     * Adds a particle to the correct Cell within the CelllistContainer
     * @param p The particle to be added
     */
    void add(Particle& p);

    /**
     * Calculates how many particles are in the CelllistContainer
     * @param withHalo boolean value to specify if Particles in the halo are to be counted or not
     *
     */
    int getSize(bool withHalo);

    /**
     * Calculates how many particles are in the CelllistContainer omitting Particles in the halo
     * The same as calling getSize(false)
     *
     */
    int getSize(void) {return getSize(false);};

    /**
     * deletes all particles in the halo region
     */
    void clearHalo();

};

#endif /* CELLLISTCONTAINER_H_ */
