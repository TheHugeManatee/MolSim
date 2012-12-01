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

void ParticleGenerator::generateSphere(ParticleContainer& container, utils::Vector<double, 3> center, int radiusSphere,
										double h, double m , int type, utils::Vector<double, 3> initialVelocity, double brownianMean){

	LOG4CXX_INFO(ParticleGenerator::logger, "Generating " << (4 * radiusSphere*radiusSphere) << " Particles on a sphere");
	for(int heigth=0 ; heigth < radiusSphere; heigth++){
		LOG4CXX_TRACE(ParticleGenerator::logger, "Heigth: \t " << heigth);

		int radiusCircle = sqrt(radiusSphere * radiusSphere - heigth*heigth) ;
		LOG4CXX_TRACE(ParticleGenerator::logger, "New sphere radius \t" <<radiusCircle );

		for(int x1 = 0 ; x1 < radiusCircle; x1++  ){

			int boundaryCircle = sqrt(radiusCircle * radiusCircle - x1 * x1);
			LOG4CXX_TRACE(ParticleGenerator::logger, "New boundary circle \t" <<boundaryCircle );

			for(int x2 = 0 ; x2 < boundaryCircle; x2++){
			utils::Vector<double,3> xParticle1;
			utils::Vector<double,3> xParticle2;
			utils::Vector<double,3> xParticle3;
			utils::Vector<double,3> xParticle4;
			utils::Vector<double,3> xParticle5;
			utils::Vector<double,3> xParticle6;
			utils::Vector<double,3> xParticle7;
			utils::Vector<double,3> xParticle8;

			xParticle1[0] = center[0] + h * heigth;
			xParticle1[1] = center[1] + h * x1;
			xParticle1[2] = center[2] + h * x2;

			xParticle2[0] = center[0] - h * heigth;
			xParticle2[1] = center[1] + h * x1;
			xParticle2[2] = center[2] + h * x2;

			xParticle3[0] = center[0] + h * heigth;
			xParticle3[1] = center[1] - h * x1;
			xParticle3[2] = center[2] + h * x2;

			xParticle4[0] = center[0] - h * heigth;
			xParticle4[1] = center[1] - h * x1;
			xParticle4[2] = center[2] + h * x2;

			xParticle5[0] = center[0] + h * heigth;
			xParticle5[1] = center[1] + h * x1;
			xParticle5[2] = center[2] - h * x2;

			xParticle6[0] = center[0] - h * heigth;
			xParticle6[1] = center[1] + h * x1;
			xParticle6[2] = center[2] - h * x2;

			xParticle7[0] = center[0] + h * heigth;
			xParticle7[1] = center[1] - h * x1;
			xParticle7[2] = center[2] - h * x2;

			xParticle8[0] = center[0] - h * heigth;
			xParticle8[1] = center[1] - h * x1;
			xParticle8[2] = center[2] - h * x2;

			Particle p1(xParticle1 , initialVelocity, m ,type);
			Particle p2(xParticle2 , initialVelocity, m ,type);
			Particle p3(xParticle3 , initialVelocity, m ,type);
			Particle p4(xParticle4 , initialVelocity, m ,type);
			Particle p5(xParticle5 , initialVelocity, m ,type);
			Particle p6(xParticle6 , initialVelocity, m ,type);
			Particle p7(xParticle7 , initialVelocity, m ,type);
			Particle p8(xParticle8 , initialVelocity, m ,type);

			MaxwellBoltzmannDistribution(p1, brownianMean, 2);
			MaxwellBoltzmannDistribution(p2, brownianMean, 2);
			MaxwellBoltzmannDistribution(p3, brownianMean, 2);
			MaxwellBoltzmannDistribution(p4, brownianMean, 2);
			MaxwellBoltzmannDistribution(p5, brownianMean, 2);
			MaxwellBoltzmannDistribution(p6, brownianMean, 2);
			MaxwellBoltzmannDistribution(p7, brownianMean, 2);
			MaxwellBoltzmannDistribution(p8, brownianMean, 2);
			container.add(p1);
			container.add(p2);
			container.add(p3);
			container.add(p4);
			container.add(p5);
			container.add(p6);
			container.add(p7);
			container.add(p8);

			}
		}
	}
}
