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
#include <utils/Matrix.h>

log4cxx::LoggerPtr ParticleGenerator::logger = log4cxx::Logger::getLogger("ParticleGenerator");

void ParticleGenerator::regularCuboid(ParticleContainer& container, utils::Vector<double, 3> bottomLeft,
		int nX0, int nX1, int nX2,
		double h, int type,
		utils::Vector<double, 3> initialVelocity,
		double brownianMean,
		utils::Matrix transform) {

	LOG4CXX_INFO(ParticleGenerator::logger, "Generating " << (nX0*nX1*nX2) << " Particles on a regular cuboid");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "Creating Cuboid");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tbottomLeft:\t" << bottomLeft.toString());
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tnX:\t\t[" << nX0 << ";" << nX1 << ";" << nX2 << "]");
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\th:\t\t" << h);
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\ttype:\t\t" << type);
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tinitialVelocity:" << initialVelocity.toString());
	LOG4CXX_DEBUG(ParticleGenerator::logger, "\tbrownMeanVel:\t" << brownianMean);

	if(type >= Settings::numParticleTypes) {
		LOG4CXX_FATAL(logger, "Undefined Type ID: " << type);
		exit(1);
	}


	Settings::particleTypes[type].membraneDescriptor.nX0 = nX0;
	Settings::particleTypes[type].membraneDescriptor.nX1 = nX1;
	Settings::particleTypes[type].membraneDescriptor.nX2 = nX2;

	utils::Matrix moveBack = utils::Matrix::translate(bottomLeft[0] + nX0*h/2, bottomLeft[1] + nX1*h/2, bottomLeft[2] + nX2*h/2);

	for(int x0=0; x0 < nX0; x0++)
		for(int x1=0; x1 < nX1; x1++)
			for(int x2=0; x2 < nX2; x2++) {
				utils::Vector<double, 3> x;
				x[0] = (x0 - nX0/2) * h;
				x[1] = (x1 - nX1/2) * h;
				x[2] = (x2 - nX2/2) * h;

				int pid = (x2+1) + (x1+1)* (nX2+2) + (x0+1)*(nX2+2)*(nX1+2);

				//(moveBack*t*moveToOrigin).transform(x);
				(transform*moveBack).transform(x);
				Particle p(x, initialVelocity, type, pid);

				if(brownianMean != 0)
					MaxwellBoltzmannDistribution(p, brownianMean, Settings::dimensions);

				container.add(p);
			}
}

void ParticleGenerator::generateSphere(ParticleContainer& container,
		utils::Vector<double, 3> center, int radiusSphere, double h, int type,
		utils::Vector<double, 3> initialVelocity, double brownianMean,
		utils::Matrix transform) {

	if (type >= Settings::numParticleTypes) {
		LOG4CXX_FATAL(logger, "Undefined Type ID: " << type);
		exit(1);
	}

	Settings::particleTypes[type].membraneDescriptor.nX0 = radiusSphere * 2;
	Settings::particleTypes[type].membraneDescriptor.nX1 = radiusSphere * 2;
	Settings::particleTypes[type].membraneDescriptor.nX2 = radiusSphere * 2;

	double r = (radiusSphere+radiusSphere)/2;
	LOG4CXX_INFO(ParticleGenerator::logger,
			"Generating approx." << (int)(4/3 * r*r*r * 3.14159265) << " Particles on a sphere");
	std::vector<int> diffs;
	diffs.push_back(-1);
	diffs.push_back(1);
	if (Settings::dimensions == 3) {
		for (int height = 0; height < radiusSphere; height++) {
			LOG4CXX_TRACE(ParticleGenerator::logger, "Height: \t " << height);

			int radiusCircle = floor(
					sqrt(radiusSphere * radiusSphere - height * height) + 0.5);
			LOG4CXX_TRACE(ParticleGenerator::logger,
					"New sphere radius \t" <<radiusCircle);

			for (int x1 = 0; x1 < radiusCircle; x1++) {

				int boundaryCircle = floor(
						sqrt(radiusCircle * radiusCircle - x1 * x1) + 0.5);
				LOG4CXX_TRACE(ParticleGenerator::logger,
						"New boundary circle \t" <<boundaryCircle);

				for (int x2 = 0; x2 < boundaryCircle; x2++) {

					for (std::vector<int>::iterator diffX = diffs.begin();
							diffX != diffs.end(); diffX++) {
						for (std::vector<int>::iterator diffY = diffs.begin();
								diffY != diffs.end(); diffY++) {
							for (std::vector<int>::iterator diffZ =
									diffs.begin(); diffZ != diffs.end();
									diffZ++) {

								utils::Vector<double, 3> xParticle;

								xParticle[0] = (h * height) * *diffX;
								xParticle[1] = (h * x1) * *diffY;
								xParticle[2] = (h * x2) * *diffZ;

								(utils::Matrix::translate(center[0],center[1],center[2]) * transform).transform(xParticle);

								int tempRadius = radiusSphere + 1;
								int pid = tempRadius + (*diffZ) * (x2) +
										2*tempRadius * (tempRadius + (*diffY) * (x1)) +
										4*tempRadius*tempRadius * (tempRadius + (*diffX) * (height));

								pid += 1 + 2 * tempRadius + 4*tempRadius*tempRadius;
								Particle p(xParticle, initialVelocity, type, pid);

								if (brownianMean != 0)
									MaxwellBoltzmannDistribution(p,
											brownianMean, Settings::dimensions);
								container.add(p);
								if (x2 == 0)
									break;
							}
							if (x1 == 0)
								break;
						}
						if (height == 0)
							break;
					}
				}
			}
		}
	} else {
		for (int height = 0; height < radiusSphere; height++) {
			LOG4CXX_TRACE(ParticleGenerator::logger, "Height: \t " << height);
			int radiusCircle = floor(
					sqrt(radiusSphere * radiusSphere - height * height) + 0.5);
			LOG4CXX_TRACE(ParticleGenerator::logger,
					"New sphere radius \t" <<radiusCircle);
			for (int x1 = 0; x1 < radiusCircle; x1++) {
				int boundaryCircle = floor(
						sqrt(radiusCircle * radiusCircle - x1 * x1) + 0.5);
				LOG4CXX_TRACE(ParticleGenerator::logger,
						"New boundary circle \t" <<boundaryCircle);
				for (std::vector<int>::iterator diffX = diffs.begin();
						diffX != diffs.end(); diffX++) {
					for (std::vector<int>::iterator diffY = diffs.begin();
							diffY != diffs.end(); diffY++) {
						utils::Vector<double, 3> xParticle;
						xParticle[0] = center[0] + (h * height) * *diffX;
						xParticle[1] = center[1] + (h * x1) * *diffY;
						xParticle[2] = center[2];

						int pid = x1 * (*diffY ) + radiusCircle + (height * (*diffX ) + radiusCircle)*radiusCircle*2 ;
						Particle p(xParticle, initialVelocity, type, pid);

						if (brownianMean != 0)
							MaxwellBoltzmannDistribution(p, brownianMean,
									Settings::dimensions);
						container.add(p);
						if (x1 == 0)
							break;
					}
					if (height == 0)
						break;
				}
			}
		}

	}

}

void ParticleGenerator::generateCylinder(ParticleContainer& container,
		utils::Vector<double, 3> bottom, int height, int radius, double h, int type,
		utils::Vector<double, 3> initialVelocity, double brownianMean,
		utils::Matrix transform){

	if (type >= Settings::numParticleTypes) {
		LOG4CXX_FATAL(logger, "Undefined Type ID: " << type);
		exit(1);
	}

	Settings::particleTypes[type].membraneDescriptor.nX0 = height;
	Settings::particleTypes[type].membraneDescriptor.nX1 = radius * 2;
	Settings::particleTypes[type].membraneDescriptor.nX2 = radius * 2;

	LOG4CXX_INFO(ParticleGenerator::logger,
			"Generating " << (4 * radius*radius*height) << " Particles on a cylinder");
	std::vector<int> diffs;
	diffs.push_back(-1);
	diffs.push_back(1);


		for (int h1 = 0; h1 < height; h1++) {
			LOG4CXX_TRACE(ParticleGenerator::logger, "Height: \t " << height);

			for (int x1 = 0; x1 < radius; x1++) {

				int boundaryCircle = floor(
						sqrt(radius * radius - x1 * x1) + 0.5);
				LOG4CXX_TRACE(ParticleGenerator::logger,
						"New boundary circle \t" <<boundaryCircle);

				for (int x2 = 0; x2 < boundaryCircle; x2++) {

						for (std::vector<int>::iterator diffY = diffs.begin();
								diffY != diffs.end(); diffY++) {
							for (std::vector<int>::iterator diffZ =
									diffs.begin(); diffZ != diffs.end();
									diffZ++) {

								utils::Vector<double, 3> xParticle;
								xParticle = bottom;
								xParticle[0] += (h * h1);
								xParticle[1] += (h * x1) * *diffY;
								xParticle[2] += (h * x2) * *diffZ;

								int tempRadius = radius + 1;

								int pid = (tempRadius + (*diffZ) * (x2)) +
										2*tempRadius * (tempRadius + (*diffY) * (x1)) +
										4*tempRadius * tempRadius * (h1);

								pid += 1+ 2* tempRadius + 4* tempRadius* tempRadius;
								Particle p(xParticle, initialVelocity, type, pid);

								if (brownianMean != 0)
									MaxwellBoltzmannDistribution(p,
											brownianMean, Settings::dimensions);
								container.add(p);
								if (x2 == 0)
									break;
							}
							if (x1 == 0)
								break;
					}
				}
			}
		}
}


void ParticleGenerator::performGeneration(ParticleContainer &container) {
	LOG4CXX_TRACE(logger, "Cuboid generation:");
	for(auto it = Settings::generator.cuboid().begin();
			it != Settings::generator.cuboid().end();
			++it) {

		auto c = (*it);
		double v[] = {c.initialVelocity().x0(), c.initialVelocity().x1(), c.initialVelocity().x2()};
		double bl[] = {c.bottomLeft().x0(), c.bottomLeft().x1(), c.bottomLeft().x2()};
		double bMean = c.brownianMeanVelocity().present()?c.brownianMeanVelocity().get():0;

		//multiply complete transformation matrix
		utils::Matrix transformation;
		for(auto it = c.transform().begin(); it != c.transform().end(); ++it) {
			Transform transDef = *it;
			transformation = utils::Matrix::buildFromXml(transDef) * transformation;
		}

		ParticleGenerator::regularCuboid(container,
				utils::Vector<double, 3> (bl),
				c.nX().x0(), c.nX().x1(), c.nX().x2(),
				c.stepWidth(), c.type(),
				utils::Vector<double, 3> (v),
				bMean,
				transformation
		);

	}

	LOG4CXX_TRACE(logger, "Cylinder generation:");
	for(auto it = Settings::generator.cylinder().begin();
			it != Settings::generator.cylinder().end();
			++it) {

		auto c = (*it);
		double v[] = {c.initialVelocity().x0(), c.initialVelocity().x1(), c.initialVelocity().x2()};
		double bottom[] = {c.bottom().x0(), c.bottom().x1(), c.bottom().x2()};
		double bMean = c.brownianMeanVelocity().present()?c.brownianMeanVelocity().get():0;

		//multiply complete transformation matrix
		utils::Matrix transformation;
		for(auto it = c.transform().begin(); it != c.transform().end(); ++it) {
			Transform transDef = *it;
			transformation = utils::Matrix::buildFromXml(transDef) * transformation;
		}
		ParticleGenerator::generateCylinder(container,
				utils::Vector<double, 3> (bottom),
				c.height(),
				c.radius(),
				c.stepWidth(), c.type(),
				utils::Vector<double, 3> (v),
				bMean,
				transformation
		);

	}

	LOG4CXX_TRACE(logger, "Sphere generation:");

	for(auto it = Settings::generator.sphere().begin();
			it != Settings::generator.sphere().end();
			++it) {

		auto c = (*it);
		double v[] = {c.initialVelocity().x0(), c.initialVelocity().x1(), c.initialVelocity().x2()};
		double center[] = {c.center().x0(), c.center().x1(), c.center().x2()};
		double bMean = c.brownianMeanVelocity().present()?c.brownianMeanVelocity().get():0;

		//multiply complete transformation matrix
		utils::Matrix transformation;
		for(auto it = c.transform().begin(); it != c.transform().end(); ++it) {
			Transform transDef = *it;
			transformation = utils::Matrix::buildFromXml(transDef) * transformation;
		}
		ParticleGenerator::generateSphere(container,
				utils::Vector<double, 3> (center),
				c.radius(),
				c.stepWidth(), c.type(),
				utils::Vector<double, 3> (v),
				bMean,
				transformation
		);

	}
	LOG4CXX_TRACE(logger, "Generation finished!");
}
