/**
 * @file ParticleContainer.h
 *
 * container class for all particles in the simulation
 * offering methods to apply a function to all particles or particle pairs
 *
 * @author Jakob Weiss, Alexander Winkler, Leonhard Rannabauer
 * @date 01.11.2012
 *
 */


#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include <vector>
#include <functional>

#include "Particle.h"


class ParticleContainer {
    private:
        std::vector<Particle> particles;

    public:

        /**
         * to iterate a given function over all particles from the particles list
         * @param: fn an anonymous function (e.g numerical force calculation)
         */
        void each(std::function<void (Particle&)> fn);


        /**
         * calls the given lambda expression for every unique pair of particles
         * permutations of two particles as well as pairs of the same particle
         * are ignored, that means that for n particles, n^² - n calls will be made
         * @param: fn a lambda expression
         */
        void eachPair(std::function<void (Particle&, Particle&)> fn);

        /**
         * returns the internal container
         */
        std::vector<Particle>& getContainer();
};

#endif

