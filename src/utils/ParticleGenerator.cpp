/*
 * ParticleGenerator.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ParticleGenerator.h"

#include "MaxwellBoltzmannDistribution.h"
#include <utils/Settings.h>
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

	double sigma = Settings::sigma;
	double epsilon = Settings::epsilon;

	for(auto it = Settings::particleTypes.particleType().begin() ; it != Settings::particleTypes.particleType().end() ; ++it){
		auto c = (*it);
		if(c.Nr() == type){
			sigma = c.sigma();
			epsilon = c.epsilon();
		}
	}

	for(int x1=0; x1 < nX1; x1++)
		for(int x2=0; x2 < nX2; x2++)
			for(int x3=0; x3 < nX3; x3++) {
				utils::Vector<double, 3> x;
				x[0] = bottomLeft[0] + x1 * h;
				x[1] = bottomLeft[1] + x2 * h;
				x[2] = bottomLeft[2] + x3 * h;

				Particle p(x, initialVelocity, m, type,1 ,1);

				if(brownianMean != 0)
				MaxwellBoltzmannDistribution(p, brownianMean, Settings::dimensions);

				container.add(p);
			}
}

void ParticleGenerator::generateSphere(ParticleContainer& container, utils::Vector<double, 3> center, int radiusSphere,
										double h, double m , int type, utils::Vector<double, 3> initialVelocity, double brownianMean){

	double sigma = Settings::sigma;
	double epsilon = Settings::epsilon;

	for(auto it = Settings::particleTypes.particleType().begin() ; it != Settings::particleTypes.particleType().end() ; ++it){
		auto c = (*it);
		if(c.Nr() == type){
			sigma = c.sigma();
			epsilon = c.epsilon();
		}
	}
	LOG4CXX_INFO(ParticleGenerator::logger, "Generating " << (4 * radiusSphere*radiusSphere) << " Particles on a sphere");
	std::vector<int> diffs;
	diffs.push_back(-1);
	diffs.push_back(1);

	for(int height=0 ; height < radiusSphere; height++){
		LOG4CXX_TRACE(ParticleGenerator::logger, "Height: \t " << height);

		int radiusCircle = floor(sqrt(radiusSphere * radiusSphere - height*height)+0.5) ;
		LOG4CXX_TRACE(ParticleGenerator::logger, "New sphere radius \t" <<radiusCircle );

		for(int x1 = 0 ; x1 < radiusCircle; x1++  ){

			int boundaryCircle = floor(sqrt(radiusCircle * radiusCircle - x1 * x1) + 0.5);
			LOG4CXX_TRACE(ParticleGenerator::logger, "New boundary circle \t" <<boundaryCircle );

			for(int x2 = 0 ; x2 < boundaryCircle; x2++){

				for(std::vector<int>::iterator diffX = diffs.begin() ; diffX != diffs.end(); diffX++){
					for(std::vector<int>::iterator diffY = diffs.begin() ; diffY != diffs.end(); diffY++){
						for(std::vector<int>::iterator diffZ = diffs.begin() ; diffZ != diffs.end(); diffZ++){

							utils::Vector<double,3> xParticle;


							xParticle[0] = center[0] + (h * height) * *diffX ;
							xParticle[1] = center[1] + (h * x1) * *diffY ;
							xParticle[2] = center[2];
							if(Settings::dimensions == 3)
								xParticle[2] += (h * x2) * *diffZ ;

							Particle p(xParticle , initialVelocity, m ,type ,1 ,1);

							if(brownianMean != 0)
							MaxwellBoltzmannDistribution(p, brownianMean, Settings::dimensions);

							container.add(p);
							if((Settings::dimensions == 2)||(x2 == 0))
								break;
						}
						if(x1 == 0)
							break;
					}
					if(height == 0)
						break;
				}
			}
		}
	}
}
