#include <algorithm>
#include <cassert>

#include "ParticleContainer.h"


ParticleContainer::~ParticleContainer() {

}

void ParticleContainer::each(std::function<void (Particle&)> fn) {
	int s = particles.size();
	for(int i = 0; i < s; i++) {
		fn(particles[i]);
	}
}

void ParticleContainer::eachPair(std::function<void (Particle&, Particle&)> fn) {
	std::vector<Particle>::iterator iterator;
	int s = particles.size();
	for(int i = 0; i < s; i++) {
		for(int j = i + 1; j < s; j++) {
			fn(particles[i], particles[j]);
		}
	}
}

void ParticleContainer::add(Particle& p) {
	particles.push_back(p);
}


int ParticleContainer::getSize()
{
	return particles.size();
}
