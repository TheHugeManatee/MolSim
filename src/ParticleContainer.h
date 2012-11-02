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
         * to iterate a given function over all permutations of two particles from the particles list
         * permutations containing identical particles are only called once
         * @param: fn an anonymous function (e.g numerical force calculation)
         */

        void eachPair(std::function<void (Particle&, Particle&)> fn);
        std::vector<Particle>& getContainer();
};

#endif

