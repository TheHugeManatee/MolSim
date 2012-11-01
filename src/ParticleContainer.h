#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include <list>
#include <functional>

#include "Particle.h"

class ParticleContainer {
    private:
        std::list<Particle> particles;

    public:
        void each(std::function<void (Particle&)> fn);

        void eachPair(std::function<void (Particle&, Particle&)> fn);
        std::list<Particle>& getList();
};

#endif

