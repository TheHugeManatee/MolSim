/*
 * ParticleGenerator.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ParticleGenerator.h"

#include "MaxwellBoltzmannDistribution.h"

#include <log4cxx/logger.h>


log4cxx::LoggerPtr ParticleGenerator::logger = log4cxx::Logger::getLogger("ParticleGenerator");

void ParticleGenerator::regularCuboid(ParticleContainer& container, utils::Vector<double, 3> bottomLeft,
		int nX1, int nX2, int nX3,
		double h, double m, int type,
		utils::Vector<double, 3> initialVelocity,
		double brownianMean) {

	LOG4CXX_INFO(ParticleGenerator::logger, "Generating " << (nX1*nX2*nX3) << " Particles on a regular cuboid");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "Creating Cuboid");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tbottomLeft:\t" << bottomLeft.toString());
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tnX:\t\t[" << nX1 << ";" << nX2 << ";" << nX3 << "]");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\th:\t\t" << h);
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tm:\t\t" << m);
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\ttype:\t\t" << type);
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tinitialVelocity:" << initialVelocity.toString());
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tbrownMeanVel:\t" << brownianMean);


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
