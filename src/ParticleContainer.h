/**
 * @file ParticleContainer.h
 *
 * @class ParticleContainer
 *
 * @author Jakob Weiss, Alexander Winkler, Leonhard Rannabauer
 * @date 01.11.2012
 */


#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include "Particle.h"

#include <vector>
#include <functional>



/**
 * @class ParticleContainer
 *
 * This class implements a naive container for Particles and methods to apply functions on all particles
 * or all pairs of particles
 *
 * As there are O(n^2) pairs of particles this implementation has a complexity of O(n^2). Whenever possible
 * the CellistContainer should be used instead that inherits from this class and by a good approximation of
 * reality offers a complexity of O(n)
 *
 * The graph illustrates the benefit the CelllistContainer yields compared to the naive implementation in a simulation of 10 seconds
 *
 * \image html https://dl.dropbox.com/u/16135288/MolSim/ComputationTimeCelllistcontainer.png
 *
 *
 */
class ParticleContainer {
	friend class CellListContainer;
	friend class Job;
	friend class SliceJobX0;
	friend class BlockJobX0;
	friend class APCSliceJob;


    protected:
        std::vector<Particle> particles;

    public:

        /**
         * virtual destructor to accomodate virtual methods
         */
        virtual ~ParticleContainer();

        /**
         * to iterate a given function over all particles from the particles list
         * @param fn an anonymous function (e.g numerical force calculation)
         */
        virtual void each(std::function<void (Particle&)> fn);


        /**
         * calls the given lambda expression for every unique pair of particles
         * permutations of two particles as well as pairs of the same particle
         * are ignored, that means that for n particles, n^² - n calls will be made
         * @param fn a lambda expression
         */
        virtual void eachPair(std::function<void (Particle&, Particle&)> fn);

        /**
         * adds a particle to the container
         */
        virtual void add(Particle& p);

        virtual int getSize();

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
        virtual void afterPositionChanges(
        			std::function<bool (ParticleContainer &container, Particle &p)> boundaryHandlers[6]) {

        };

        virtual void clearHalo();
};

#endif
