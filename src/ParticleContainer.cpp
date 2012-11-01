#include "ParticleContainer.h"



void ParticleContainer::each(std::function<void (Particle&)> fn) {
	std::list<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {

		fn(*iterator);

		++iterator;
	}
}


void ParticleContainer::eachPair(std::function<void (Particle&, Particle&)> fn) {
	std::list<Particle>::iterator iterator;
	iterator = particles.begin();

	while (iterator != particles.end()) {
		std::list<Particle>::iterator innerIterator = particles.begin();

		while (innerIterator != particles.end()) {
			if (innerIterator != iterator) {
                fn(*iterator, *innerIterator);
			}
			++innerIterator;
		}

		++iterator;
	}
}

std::list<Particle>& ParticleContainer::getList() {
    return particles;
}
