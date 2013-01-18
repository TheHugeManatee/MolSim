#include <algorithm>
#include <cassert>

#include "ParticleContainer.h"

//base virtual destructor
ParticleContainer::~ParticleContainer() {}

void ParticleContainer::each(std::function<void (Particle&)> fn) {
	int s = particles.size();
	for(int i = 0; i < s; i++) {
		fn(particles[i]);
	}
}

void ParticleContainer::eachPair(std::function<void (Particle&, Particle&)> fn) {

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


inline int ParticleContainer::getSize()
{
	return particles.size();
}
