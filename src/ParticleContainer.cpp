#include <algorithm>
#include <cassert>
#include "ParticleContainer.h"

void ParticleContainer::each(std::function<void (Particle&)> fn) {
	assert(particles.size()!=0);

	for(int i = 0; i < particles.size(); i++) {
		fn(particles[i]);
	}
}

void ParticleContainer::eachPair(std::function<void (Particle&, Particle&)> fn) {
	std::vector<Particle>::iterator iterator;
	assert(particles.size()!=0);

	for(int i = 0; i < particles.size(); i++) {
		for(int j = i + 1; j < particles.size(); j++) {
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
