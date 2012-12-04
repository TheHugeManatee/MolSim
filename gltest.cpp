#include <iostream>
#include "ParticleContainer.h"
#include "outputWriter/RenderOutputWriter.h"
#include <GL/freeglut.h>

int main(int argc, char* argv[]) {
	RenderOutputWriter o;

	ParticleContainer ps;
	Particle p;
	for(int i=0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {

			p.x[0] = i;
			p.x[1] = j;
			p.m = i + j;
			p.x[2] = (i + j) % 3;
			ps.add(p);
		}
	}

	o.plotParticles(ps, "bla", 1);

	std::string line;

	while(1) {
		std::cin >> line;
		if(!line.empty()) {
			if(!line.compare("frame")) {
				int particleCount;
				ParticleContainer c;
				Particle p;
				std::cout << "New Frame:";
				std::cin >> particleCount;

				for(int i=0; i < particleCount; i++) {
					std::cin >> p.x[0];
					std::cin >> p.x[1];
					std::cin >> p.x[2];
					c.add(p);
				}
				o.plotParticles(c, "blabla", 1);

			}
			if(!line.compare("quit"))
				break;
		}
	}
}
