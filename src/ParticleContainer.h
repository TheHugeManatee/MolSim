/**
 * @file ParticleContainer.h
 *
 * @class ParticleContainer
 *
 * @author Jakob Weiss, Alexander Winkler, Leonhard Rannabauer
 * @date 01.11.2012
 *
 * container class for all particles in the simulation
 * offering methods to apply a function to all particles or particle pairs
 */


#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include "Particle.h"


#include <vector>
#include <functional>



class ParticleContainer {
	friend class CellListContainer;

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
         */
        virtual void afterPositionChanges(
        			std::function<bool (ParticleContainer &container, Particle &)> boundaryHandler,
        			std::function<bool (ParticleContainer &container, Particle &)> haloHandler
        ) {
        	int s = particles.size();
        	for(int i=0; i < s; i++) {
        		particles[i].old_f = particles[i].f;
        		particles[i].f = 0;
        	}
        };
};

#endif
