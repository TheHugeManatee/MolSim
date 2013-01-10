/*
 * ScenarioFactory.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ScenarioFactory.h"

#include "utils/ParticleGenerator.h"

#include "utils/Settings.h"
#include "utils/Vector.h"
#include <cstdlib>
#include <cassert>
#include <float.h>
#include <math.h>

#define TWORAISED1_6 	1.12246204830937298143353304967917
#define SQRTTWO 		1.41421356237309504880168872420969
#define SQRTTHREE 		1.73205080756887729352744634150587

log4cxx::LoggerPtr ScenarioFactory::logger = log4cxx::Logger::getLogger(
		"ScenarioFactory");

std::function<void(Particle&)> ScenarioFactory::verletUpdatePosition =
		[] (Particle& p) {
			double dt = Settings::deltaT;

			utils::Vector<double, 3> resultX;
			resultX= p.x + dt * p.v + dt * dt / (2 * Settings::particleTypes[p.type].mass) * p.f;
			p.x = resultX;
		};

std::function<void(Particle&)> ScenarioFactory::verletUpdateVelocity =
		[] (Particle& p) {
			utils::Vector<double, 3> resultV;
			resultV = p.v + Settings::deltaT / (2 * Settings::particleTypes[p.type].mass) * (p.old_f + p.f);
			p.v = resultV;
		};

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateLennardJonesPotentialForce =
		[] (Particle& p1, Particle& p2) {
			utils::Vector<double, 3> xDif = p2.x - p1.x;
			double epsilon = Settings::geometricMeanEpsilon[p1.type + p2.type * Settings::numParticleTypes];
			double sigma = (Settings::particleTypes[p1.type].sigma + Settings::particleTypes[p2.type].sigma) / 2;
			//double norm = xDif.L2Norm();
			double normSquared = xDif.LengthOptimizedR3Squared();
			double sigmaNormalizedSquared = sigma*sigma/normSquared;
			double sigmaNormailzedRaisedBySix = sigmaNormalizedSquared*sigmaNormalizedSquared*sigmaNormalizedSquared;
			utils::Vector<double, 3> resultForce = (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix))*xDif;
			p1.f = p1.f + resultForce;

			//resultForce = resultForce * -1;
			p2.f = p2.f - resultForce;
		};

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateMembraneForce =
		[] (Particle& p1, Particle& p2) {
			utils::Vector<double, 3> xDif = p2.x - p1.x;

			double epsilon = Settings::geometricMeanEpsilon[p1.type + p2.type * Settings::numParticleTypes];

			double sigma = (Settings::particleTypes[p1.type].sigma + Settings::particleTypes[p2.type].sigma) / 2;

			//double norm = xDif.L2Norm();
			double normSquared = xDif.LengthOptimizedR3Squared();
			double sigmaNormalizedSquared = sigma*sigma/normSquared;
			double sigmaNormailzedRaisedBySix = sigmaNormalizedSquared*sigmaNormalizedSquared*sigmaNormalizedSquared;

			utils::Vector<double, 3> resultForce;
			resultForce = 0;

			if(Settings::particleTypes[p1.type].isMolecule && (p1.type == p2.type) && (p1.type != -1)) {
				//std::cout << "Particles in the same membrane" << std::endl;
				double F = 0, norm = 1;
				//std::cout << "Id1:" << p1.id << "Id2:"<< p2.id << std::endl;
				if(p1.isNeighbour(p2)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness * (norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength);
					//std::cout << "1:" << F << std::endl;
				} else if(p1.isFaceDiagonal(p2)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness *(norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength*SQRTTWO);
					//std::cout << "2:" << F << std::endl;
				} else if(p1.isSpaceDiagonal(p2)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness *(norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength*SQRTTHREE);
					//std::cout << "3:" << F << std::endl;
				}
				double a = F / norm;
				//std::cout << a << std::endl;
				resultForce = xDif * a;

				if(normSquared < TWORAISED1_6 * sigma * TWORAISED1_6 * sigma) { //use repulsing part of LJ-Potential
					resultForce = resultForce + (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix))*xDif;
				}

				p1.f = p1.f + resultForce;

				//resultForce = resultForce * -1;
				p2.f = p2.f - resultForce;
			} else {
				resultForce = (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix))*xDif;
				p1.f = p1.f + resultForce;

				//resultForce = resultForce * -1;
				p2.f = p2.f - resultForce;
			}

			//assert(p1.f[0] < 1000);
			//assert(p1.f[1] < 1000);
			//assert(p1.f[2] < 1000);

	};

/*std::function<void (Particle&, Particle&)> ScenarioFactory::calculateLennardJonesPotentialForce = [] (Particle& p1, Particle& p2) {
 utils::Vector<double, 3> xDif = p2.x - p1.x;
 double norm = xDif.L2Norm();
 double sigmaNormalized = Settings::sigma/norm;
 double sigmaNormailzedRaisedBySix = sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized;
 utils::Vector<double, 3> resultForce = (24*Settings::epsilon / (norm * norm)) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix))*xDif;
 p1.f = p1.f + resultForce;
 //resultForce = resultForce * -1;
 p2.f = p2.f - resultForce;
 };*/

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateGravityForce =
		[] (Particle& p1, Particle& p2) {
			utils::Vector<double, 3> xDif = p2.x - p1.x;
			double normRaised3 = xDif.L2Norm() * xDif.L2Norm() * xDif.L2Norm();
			utils::Vector<double, 3> resultForce = ((Settings::particleTypes[p1.type].mass * Settings::particleTypes[p2.type].mass) / normRaised3) * (xDif);

			p1.f = p1.f + resultForce;

			p2.f = p2.f - resultForce;
		};

std::function<void(ParticleContainer &container)> ScenarioFactory::basicFileReaderSetup =
		[](ParticleContainer& container) {
			LOG4CXX_DEBUG(logger, "Performing basic scenario setup");
			if(!Settings::inputFile.empty()) {
				FileReader fileReader;
				fileReader.readFile(container, (char*)Settings::inputFile.c_str());
			}
		};

std::function<void(ParticleContainer &container)> ScenarioFactory::LennardJonesSetup =
		[](ParticleContainer& container) {
			LOG4CXX_DEBUG(logger, "Performing Lennard-Jones setup");
			if(!Settings::inputFile.empty()) {
				FileReader fileReader;
				fileReader.readFile(container, (char*)Settings::inputFile.c_str());
			}
			LOG4CXX_TRACE(logger, "Cuboid generation:");
			for(auto it = Settings::generator.cuboid().begin();
					it != Settings::generator.cuboid().end();
					++it) {

				auto c = (*it);
				double v[] = {c.initialVelocity().x0(), c.initialVelocity().x1(), c.initialVelocity().x2()};
				double bl[] = {c.bottomLeft().x0(), c.bottomLeft().x1(), c.bottomLeft().x2()};
				auto brown_opt = c.brownianMeanVelocity();
				if(brown_opt.present()) {
					ParticleGenerator::regularCuboid(container,
							utils::Vector<double, 3> (bl),
							c.nX().x0(), c.nX().x1(), c.nX().x2(),
							c.stepWidth(), c.type(),
							utils::Vector<double, 3> (v),
							brown_opt.get()
					);
				} else {
					ParticleGenerator::regularCuboid(container,
							utils::Vector<double, 3> (bl),
							c.nX().x0(), c.nX().x1(), c.nX().x2(),
							c.stepWidth(), c.type(),
							utils::Vector<double, 3> (v),
							0);
				}
			}
			LOG4CXX_TRACE(logger, "Generation finished!");

			for(auto it = Settings::generator.sphere().begin();
					it != Settings::generator.sphere().end();
					++it) {

				auto c = (*it);
				double v[] = {c.initialVelocity().x0(), c.initialVelocity().x1(), c.initialVelocity().x2()};
				double center[] = {c.center().x0(), c.center().x1(), c.center().x2()};
				auto brown_opt = c.brownianMeanVelocity();
				if(brown_opt.present()) {
					ParticleGenerator::generateSphere(container,
							utils::Vector<double, 3> (center),
							c.radius(),
							c.stepWidth(), c.type(),
							utils::Vector<double, 3> (v),
							c.brownianMeanVelocity().get()
					);
				} else {
					ParticleGenerator::generateSphere(container,
							utils::Vector<double, 3> (center),
							c.radius(),
							c.stepWidth(), c.type(),
							utils::Vector<double, 3> (v),
							0);
				}
			}

		};

/*These are the handlers for a periodic boundaryHandling
 * Particles in boundary cells have to be copied to the opposite halo cell for forceCalculations on relying cells  */

std::function<bool(ParticleContainer &, Particle &p)> ScenarioFactory::periodicHandlers[] =
		{
		//x0 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[0] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[0] >= 0) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = realDomainSize + p.x[0];
						positionInHalo[1] = p.x[1];
						positionInHalo[2] = p.x[2];
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[0] < 0) { //wrap particle around if we are outside the domain
						p.x[0] = realDomainSize + p.x[0];
					}
					return false;

				},
				//x0 = domain[0] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[0] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[0] <= realDomainSize) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = p.x[0] - realDomainSize;
						positionInHalo[1] = p.x[1];
						positionInHalo[2] = p.x[2];
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[0] > realDomainSize) { //wrap particle around if we are outside the domain
						p.x[0] = p.x[0] - realDomainSize;
					}
					return false;
				},
				//x1 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[1] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[1] >= 0) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = p.x[0];
						positionInHalo[1] = p.x[1] + realDomainSize;
						positionInHalo[2] = p.x[2];
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[1] < 0) { //wrap particle around if we are outside the domain
						p.x[1] = realDomainSize + p.x[1];
					}
					return false;
				},
				//x1 = domain[1] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[1] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[1] <= realDomainSize) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = p.x[0];
						positionInHalo[1] = p.x[1] - realDomainSize;
						positionInHalo[2] = p.x[2];
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[1] > realDomainSize) { //wrap particle around if we are outside the domain
						p.x[1] = p.x[1] - realDomainSize;
					}

					return false;
				},
				//x2 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[2] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[2]>= 0 ) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = p.x[0];
						positionInHalo[1] = p.x[1];
						positionInHalo[2] = realDomainSize + p.x[2];
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[2] < 0) { //wrap particle around if we are outside the domain
						p.x[2] = realDomainSize + p.x[2];
					}
					return false;
				},
				//x2 = domain[2] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = std::ceil(Settings::domainSize[2] / Settings::rCutoff) * Settings::rCutoff;
					if(p.x[2]<= realDomainSize ) {
						zeroVelocity [0] = 0;
						zeroVelocity [1] = 0;
						zeroVelocity [2] = 0;
						positionInHalo[0] = p.x[0];
						positionInHalo[1] = p.x[1];
						positionInHalo[2] = p.x[2] - realDomainSize;
						Particle pNew (positionInHalo, zeroVelocity, p.type, p.id);
						container.add(pNew);
					}
					if(p.x[2] > realDomainSize) { //we have a litte problem around here: particles with x[i] > Settings::domainSize[i] are deleted accoding to the HaloHandler
						p.x[2] = p.x[2] - realDomainSize;//
					}
					return false;

				} };

SimulationScenario *ScenarioFactory::build(ScenarioType type) {
	SimulationScenario *scenario = new SimulationScenario;
	if (type == ScenarioType::Gravity) {
		//Simple gravity simulation with gravitational constant g = 1
		scenario->calculateForce = ScenarioFactory::calculateGravityForce;

		scenario->setup = ScenarioFactory::basicFileReaderSetup;

		scenario->updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;
		LOG4CXX_DEBUG(logger, "Built Gravity Scenario");
	} else if (type == ScenarioType::Lennard_Jones) {
		scenario->calculateForce =
				ScenarioFactory::calculateLennardJonesPotentialForce;

		scenario->setup = ScenarioFactory::LennardJonesSetup;

		scenario->updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;

		LOG4CXX_DEBUG(logger, "Built Lennard-Jones Scenario");
	} else if (type == ScenarioType::Membrane) {
		scenario->calculateForce = ScenarioFactory::calculateMembraneForce;

		scenario->setup = ScenarioFactory::LennardJonesSetup;

		scenario->updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;
	} else {
		LOG4CXX_FATAL(logger, "Unknown Simulation type!");
		exit(-1);
	}

	//For now, the halo handler always deletes the particles
	//TODO: make HaloHandling chooseable by Halo position equally to boundaryHandling
	//Needed for a periodic Boundary Handling
	//
	scenario->haloHandler = [] (ParticleContainer &container, Particle &p) {
		return true; //delete all halo particles
		};

	//these are the boundary handlers for the reflect condition
	//this is ugly but they have to be defined here so we can capture
	//the scenario->calculateForce closure from the current context
	static auto calcForce = scenario->calculateForce;
	//phantom particle to be reused to avoid constructing/destructing particles in every closure call
	static Particle phantom;
	//FYI: this is an array of closures, initialized inline. c++11 is awesome :)
	std::function<bool(ParticleContainer &, Particle &p)> reflectHandlers[] =
			{
			//x0 = 0 boundary, "left"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						if(p.x[0] < TWORAISED1_6 * Settings::particleTypes[p.type].sigma) {
							phantom.x[0] = 0;
							phantom.x[1] = p.x[1];
							phantom.x[2] = p.x[2];
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					},
					//x0 = domain[0] boundary, "right"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = std::ceil(Settings::domainSize[0] / Settings::rCutoff) * Settings::rCutoff;
						if(p.x[0] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = realDomainSize;
							phantom.x[1] = p.x[1];
							phantom.x[2] = p.x[2];
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					},
					//x1 = 0 boundary, "bottom"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						if(p.x[1] < TWORAISED1_6 * Settings::particleTypes[p.type].sigma) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = 0;
							phantom.x[2] = p.x[2];
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					},
					//x1 = domain[1] boundary, "top"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = std::ceil(Settings::domainSize[1] / Settings::rCutoff) * Settings::rCutoff;
						if(p.x[1] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = realDomainSize;
							phantom.x[2] = p.x[2];
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					},
					//x2 = 0 boundary, "back"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						if(p.x[2] < TWORAISED1_6 * Settings::particleTypes[p.type].sigma) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = p.x[1];
							phantom.x[2] = 0;
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					},
					//x2 = domain[2] boundary, "front"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = std::ceil(Settings::domainSize[2] / Settings::rCutoff) * Settings::rCutoff;
						if(p.x[2] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = p.x[1];
							phantom.x[2] = realDomainSize;
							ScenarioFactory::calculateLennardJonesPotentialForce(p, phantom);
						}
						return false;
					} };

	LOG4CXX_TRACE(logger, "Determining boundary conditions..");

	for (int i = 0; i < 6; i++) {

		switch (Settings::boundaryCondition[i]) {
		case BoundaryConditionType::Outflow:
			LOG4CXX_DEBUG(logger, "Condition " << i << " is outflow");
			scenario->boundaryHandlers[i] =
					[] (ParticleContainer &container, Particle &p) {
						//do nothing until it reaches the halo
						return false;
					};
			break;
		case BoundaryConditionType::Reflect:
			LOG4CXX_DEBUG(logger, "Condition " << i << " is reflect");

			scenario->boundaryHandlers[i] = reflectHandlers[i];

			break;
		case BoundaryConditionType::Periodic:
			LOG4CXX_DEBUG(logger, "Condition " << i << " is Periodic");
			scenario->boundaryHandlers[i] = periodicHandlers[i];

			break;
		}
	};
	LOG4CXX_TRACE(logger, "Boundary condition established..");

	return scenario;
}
