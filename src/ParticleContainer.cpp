#include <algorithm>
#include <cassert>

#include "ParticleContainer.h"
#include "utils/Settings.h"

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

void ParticleContainer::clearHalo() {
	int s = particles.size();
	for(int i=0; i < s; i++) {
		Particle &p = particles[i];

		if(p.x[0] < 0 || p.x[0] > Settings::domainSize[0] ||
			p.x[1] < 0 || p.x[1] > Settings::domainSize[1] ||
			p.x[2] < 0 || p.x[2] > Settings::domainSize[2]) {

			particles[i] = particles[s-s];
			particles.pop_back();

			s--;
			i--;
		}
	}
}
