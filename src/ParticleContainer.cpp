#include <algorithm>

#include "ParticleContainer.h"



void ParticleContainer::each(std::function<void (Particle&)> fn) {
	//std::for_each(particles.begin(), particles.end(), fn);

	for(int i = 0; i < particles.size(); i++) {
		fn(particles[i]);
	}
	/*std::vector<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {

		fn(*iterator);

		++iterator;
	}*/
}


void ParticleContainer::eachPair(std::function<void (Particle&, Particle&)> fn) {
	std::vector<Particle>::iterator iterator;

	for(int i = 0; i < particles.size(); i++) {
		for(int j = i + 1; j < particles.size(); j++) {
			fn(particles[i], particles[j]);
		}
	}
}

std::vector<Particle>& ParticleContainer::getContainer() {
    return particles;
}
