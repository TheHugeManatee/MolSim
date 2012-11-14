/*
 * ParticleGenerator.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ParticleGenerator.h"

#include "MaxwellBoltzmannDistribution.h"



void generateParticlesRegularCuboid(ParticleContainer& container, utils::Vector<double, 3> bottomLeft,
		int nX1, int nX2, int nX3,
		double h, double m, int type,
		utils::Vector<double, 3> initialVelocity,
		double brownianMean) {

	for(int x1=0; x1 < nX1; x1++)
		for(int x2=0; x2 < nX2; x2++)
			for(int x3=0; x3 < nX3; x3++) {
				utils::Vector<double, 3> x;
				x[0] = bottomLeft[0] + x1 * h;
				x[1] = bottomLeft[1] + x2 * h;
				x[2] = bottomLeft[2] + x3 * h;

				Particle p(x, initialVelocity, m, type);
				MaxwellBoltzmannDistribution(p, brownianMean, 2);

				container.add(p);
			}
}
