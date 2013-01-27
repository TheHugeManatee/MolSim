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
#include "utils/Thermostat.h"
#include "Simulator.h"
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

			utils::Vector<double, 3> f;

			f[0] = p.f_acc[0][0] + p.f_acc[1][0] + p.f_acc[2][0] + p.f_acc[3][0] + p.f_acc[4][0] + p.f_acc[5][0] + p.f_acc[6][0] + p.f_acc[7][0];
			f[1] = p.f_acc[0][1] + p.f_acc[1][1] + p.f_acc[2][1] + p.f_acc[3][1] + p.f_acc[4][1] + p.f_acc[5][1] + p.f_acc[6][1] + p.f_acc[7][1];
			f[2] = p.f_acc[0][2] + p.f_acc[1][2] + p.f_acc[2][2] + p.f_acc[3][2] + p.f_acc[4][2] + p.f_acc[5][2] + p.f_acc[6][2] + p.f_acc[7][2];

			//new velocity
			utils::Vector<double, 3> resultV;
			resultV = p.v + dt / (2 * Settings::particleTypes[p.type].mass) * (p.old_f + f);
			p.v = resultV;

			//new position
			utils::Vector<double, 3> resultX;
			resultX= p.x + dt * p.v + dt * dt / (2 * Settings::particleTypes[p.type].mass) * f;
			p.x = resultX;

			//reset force accumulator
			p.old_f[0] = f[0];
			p.old_f[1] = f[1];
			p.old_f[2] = f[2];
			memset(p.f_acc, 0, sizeof(p.f_acc));
		};

std::function<void(Particle&)> ScenarioFactory::verletUpdatePositionThermostate =
		[] (Particle& p) {
			double dt = Settings::deltaT;

			utils::Vector<double, 3> f;

			f[0] = p.f_acc[0][0] + p.f_acc[1][0] + p.f_acc[2][0] + p.f_acc[3][0] + p.f_acc[4][0] + p.f_acc[5][0] + p.f_acc[6][0] + p.f_acc[7][0];
			f[1] = p.f_acc[0][1] + p.f_acc[1][1] + p.f_acc[2][1] + p.f_acc[3][1] + p.f_acc[4][1] + p.f_acc[5][1] + p.f_acc[6][1] + p.f_acc[7][1];
			f[2] = p.f_acc[0][2] + p.f_acc[1][2] + p.f_acc[2][2] + p.f_acc[3][2] + p.f_acc[4][2] + p.f_acc[5][2] + p.f_acc[6][2] + p.f_acc[7][2];

			//new velocity
			utils::Vector<double, 3> resultV;
			resultV = p.v + dt / (2 * Settings::particleTypes[p.type].mass) * (p.old_f + f);
			//apply thermostate
			if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
				if(Simulator::iterations % Settings::thermostatSettings->controlInterval() == 0 )
					p.v = resultV*ThermostatDiscrete::beta;
			}

			//new position
			utils::Vector<double, 3> resultX;
			resultX= p.x + dt * p.v + dt * dt / (2 * Settings::particleTypes[p.type].mass) * f;
			p.x = resultX;

			//reset force accumulator
			p.old_f[0] = f[0];
			p.old_f[1] = f[1];
			p.old_f[2] = f[2];
			memset(p.f_acc, 0, sizeof(p.f_acc));
		};

std::function<void(Particle&)> ScenarioFactory::verletUpdateVelocity =
		[] (Particle& p) {

			utils::Vector<double, 3> f;

			f[0] = p.f_acc[0][0] + p.f_acc[1][0] + p.f_acc[2][0] + p.f_acc[3][0] + p.f_acc[4][0] + p.f_acc[5][0] + p.f_acc[6][0] + p.f_acc[7][0];
			f[1] = p.f_acc[0][1] + p.f_acc[1][1] + p.f_acc[2][1] + p.f_acc[3][1] + p.f_acc[4][1] + p.f_acc[5][1] + p.f_acc[6][1] + p.f_acc[7][1];
			f[2] = p.f_acc[0][2] + p.f_acc[1][2] + p.f_acc[2][2] + p.f_acc[3][2] + p.f_acc[4][2] + p.f_acc[5][2] + p.f_acc[6][2] + p.f_acc[7][2];

			utils::Vector<double, 3> resultV;
			resultV = p.v + Settings::deltaT / (2 * Settings::particleTypes[p.type].mass) * (p.old_f + f);
			p.v = resultV;
		};

std::function<void(Particle &)> ScenarioFactory::addAdditionalForces =
		[] (Particle &p) {
		int tid = omp_get_thread_num();
			for(int i = 0 ; i< Settings::forceFields.size() ; i++){
				if(p.type == Settings::forceFields[i].type()){
					int nX2 = Settings::particleTypes[p.type].membraneDescriptor.nX2+2;
					int nX1 = Settings::particleTypes[p.type].membraneDescriptor.nX1+2;
					int x2 = p.id % nX2 -1;
					int x1 = (p.id/nX2) % (nX1) -1 ; //int pid = x2 + x1*nX2 + x0*nX2*nX1
					int x0 = p.id / (nX2*nX1) -1;
					if(x2 >= Settings::forceFields[i].from().x2() && x1 >= Settings::forceFields[i].from().x1() &&
						x0 >= Settings::forceFields[i].from().x0() && x2 <= Settings::forceFields[i].to().x2() &&
						x1 <= Settings::forceFields[i].to().x1() && x0 <= Settings::forceFields[i].to().x0()){
						p.f_acc[0][0] += Settings::forceFields[i].force().x0();
						p.f_acc[0][1] += Settings::forceFields[i].force().x1();
						p.f_acc[0][2] += Settings::forceFields[i].force().x2();

					}
				}
			}

			utils::Vector<double, 3> gravitationalForce;
			gravitationalForce= Settings::particleTypes[p.type].mass * Settings::gravitation;
			p.f_acc[tid][0] += Settings::particleTypes[p.type].mass * Settings::gravitation[0];
			p.f_acc[tid][1] += Settings::particleTypes[p.type].mass * Settings::gravitation[1];
			p.f_acc[tid][2] += Settings::particleTypes[p.type].mass * Settings::gravitation[2];

};

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateLennardJonesPotentialForce =
		[] (Particle& p1, Particle& p2) {
			int tid = omp_get_thread_num();

			utils::Vector<double, 3> xDif = p2.x - p1.x;
			double epsilon = Settings::geometricMeanEpsilon[p1.type + p2.type * Settings::numParticleTypes];
			double sigma = (Settings::particleTypes[p1.type].sigma + Settings::particleTypes[p2.type].sigma) / 2;

			double normSquared = xDif.LengthOptimizedR3Squared();
			double sigmaNormalizedSquared = sigma*sigma/normSquared;
			double sigmaNormailzedRaisedBySix = sigmaNormalizedSquared*sigmaNormalizedSquared*sigmaNormalizedSquared;

			double magnitude = (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix));

			//utils::Vector<double, 3> resultForce = magnitude*xDif;


			//p1.f = p1.f + resultForce;
			p1.f_acc[tid][0] += magnitude * xDif[0];
			p1.f_acc[tid][1] += magnitude * xDif[1];
			p1.f_acc[tid][2] += magnitude * xDif[2];

			//resultForce = resultForce * -1;
			//p2.f = p2.f - resultForce;
			p2.f_acc[tid][0] -= magnitude * xDif[0];
			p2.f_acc[tid][1] -= magnitude * xDif[1];
			p2.f_acc[tid][2] -= magnitude * xDif[2];
		};

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateSmoothLJ =
[] (Particle& p1, Particle& p2) {
	int tid = omp_get_thread_num();

	utils::Vector<double, 3> xDif = p2.x - p1.x;
	double epsilon = Settings::geometricMeanEpsilon[p1.type + p2.type * Settings::numParticleTypes];
	double sigma = (Settings::particleTypes[p1.type].sigma + Settings::particleTypes[p2.type].sigma) / 2;
	//double norm = xDif.L2Norm();
	double normSquared = xDif.LengthOptimizedR3Squared();
	double sigmaNormalizedSquared = sigma*sigma/normSquared;
	double sigmaNormalizedRaisedBySix = sigmaNormalizedSquared*sigmaNormalizedSquared*sigmaNormalizedSquared;
	double radDif = Settings::rCutoff - Settings::rl;
	utils::Vector<double, 3> resultForce;
	double norm = sqrt(normSquared);
	if(norm < Settings::rCutoff){
		utils::Vector<double, 3> lennardJonesForce = (24*epsilon/ normSquared) * ((sigmaNormalizedRaisedBySix) - 2 * (sigmaNormalizedRaisedBySix * sigmaNormalizedRaisedBySix))*xDif;
		if(norm > Settings::rl){
		double lennardJonesPotential = 4*epsilon* ((sigmaNormalizedRaisedBySix * sigmaNormalizedRaisedBySix) - (sigmaNormalizedRaisedBySix ));
		utils::Vector<double, 3> smoothingForce = 2 / (radDif) * xDif * (-radDif + radDif * Settings::rl / norm + 3 * norm - 4 * Settings::rl - Settings::rl * Settings::rl / norm);
		double smoothing = 1- (norm -Settings::rl) * (norm -Settings::rl) * ( 3 * Settings::rCutoff - Settings::rl - 2 * norm ) / ((radDif) * (radDif) * (radDif));
		resultForce =  lennardJonesPotential * smoothingForce - lennardJonesForce * smoothing;
		}else{
		resultForce = lennardJonesForce;
		}
	}
	p1.f_acc[tid][0] += resultForce[0];
	p1.f_acc[tid][1] += resultForce[1];
	p1.f_acc[tid][2] += resultForce[2];

	//resultForce = resultForce * -1;
	p2.f_acc[tid][0] -= resultForce[0];
	p2.f_acc[tid][1] -= resultForce[1];
	p2.f_acc[tid][2] -= resultForce[2];

};

#define NEIGHBOR(id, otherId, breadth, length) ((otherId - 1 == id) || (otherId + 1 == id) || (otherId - breadth == id) || (otherId + breadth == id) || (otherId - breadth * length == id) || (otherId + breadth * length == id))
#define FACEDIAG(id, otherId, breadth, length) ((otherId - 1 - breadth == id) || (otherId + 1 + breadth == id) || (otherId - breadth + 1 == id) || (otherId + breadth - 1 == id) || (otherId - breadth * length + 1 == id) || (otherId + breadth * length - 1 == id) || (otherId - breadth * length - 1 == id) || (otherId + breadth * length + 1 == id) || (otherId - breadth * (length + 1) == id) || (otherId + breadth * (length - 1) == id) || (otherId - breadth * (length - 1) == id) || (otherId + breadth* (length + 1) == id))
#define SPACEDIAG(id, otherId, breadth, length) (otherId - 1 - breadth - breadth * length == id || otherId - 1 - breadth + breadth * length == id || otherId - 1 + breadth - breadth * length == id || otherId - 1 + breadth + breadth * length == id || otherId + 1 - breadth - breadth * length == id || otherId + 1 - breadth + breadth * length == id || otherId + 1 + breadth - breadth * length == id || otherId + 1 + breadth + breadth * length == id)

std::function<void(Particle&, Particle&)> ScenarioFactory::calculateMembraneForce =
		[] (Particle& p1, Particle& p2) {
	int tid = omp_get_thread_num();
			utils::Vector<double, 3> xDif = p2.x - p1.x;

			double epsilon = Settings::geometricMeanEpsilon[p1.type + p2.type * Settings::numParticleTypes];

			double sigma = (Settings::particleTypes[p1.type].sigma + Settings::particleTypes[p2.type].sigma) / 2;

			//double norm = xDif.L2Norm();
			double normSquared = xDif.LengthOptimizedR3Squared();
			double sigmaNormalizedSquared = sigma*sigma/normSquared;
			double sigmaNormailzedRaisedBySix = sigmaNormalizedSquared*sigmaNormalizedSquared*sigmaNormalizedSquared;

			utils::Vector<double, 3> resultForce;
			double forceMagnitude = 0;

			if(Settings::particleTypes[p1.type].isMolecule && (p1.type == p2.type) && (p1.type != -1)) {

				double F = 0, norm = 1;

				int b = Settings::particleTypes[p1.type].membraneDescriptor.nX2 +2;
				int l = Settings::particleTypes[p1.type].membraneDescriptor.nX1 +2;
				if(FACEDIAG(p1.id, p2.id, b, l)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness *(norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength*SQRTTWO);
					//std::cout << "2:" << F << std::endl;
				} else if(NEIGHBOR(p1.id, p2.id, b, l)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness * (norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength);
					//std::cout << "1:" << F << std::endl;
				} else if(SPACEDIAG(p1.id, p2.id, b, l)) {
					norm = sqrt(normSquared);
					F = Settings::particleTypes[p1.type].membraneDescriptor.stiffness *(norm - Settings::particleTypes[p1.type].membraneDescriptor.averageBondLength*SQRTTHREE);
					//std::cout << "3:" << F << std::endl;
				}

				forceMagnitude += F / norm;

				if(normSquared < TWORAISED1_6 * sigma * TWORAISED1_6 * sigma) { //use repulsing part of LJ-Potential
					forceMagnitude = (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix));
				}


			} else {
				forceMagnitude = (24*epsilon/ normSquared) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix));
			}

			resultForce = forceMagnitude * xDif;

			p1.f_acc[tid][0] += resultForce[0];
			p1.f_acc[tid][1] += resultForce[1];
			p1.f_acc[tid][2] += resultForce[2];

			//resultForce = resultForce * -1;
			p2.f_acc[tid][0] -= resultForce[0];
			p2.f_acc[tid][1] -= resultForce[1];
			p2.f_acc[tid][2] -= resultForce[2];
	};


std::function<void(Particle&, Particle&)> ScenarioFactory::calculateGravityForce =
		[] (Particle& p1, Particle& p2) {
			int tid = omp_get_thread_num();

			utils::Vector<double, 3> xDif = p2.x - p1.x;
			double normRaised3 = xDif.L2Norm() * xDif.L2Norm() * xDif.L2Norm();
			utils::Vector<double, 3> resultForce = ((Settings::particleTypes[p1.type].mass * Settings::particleTypes[p2.type].mass) / normRaised3) * (xDif);

			p1.f_acc[tid][0] += resultForce[0];
			p1.f_acc[tid][1] += resultForce[1];
			p1.f_acc[tid][2] += resultForce[2];

			//resultForce = resultForce * -1;
			p2.f_acc[tid][0] -= resultForce[0];
			p2.f_acc[tid][1] -= resultForce[1];
			p2.f_acc[tid][2] -= resultForce[2];
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
			ParticleGenerator::performGeneration(container);
		};

/*These are the handlers for a periodic boundaryHandling
 * Particles in boundary cells have to be copied to the opposite halo cell for forceCalculations on relying cells  */

std::function<bool(ParticleContainer &, Particle &p)> ScenarioFactory::periodicHandlers[] =
		{
		//x0 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = Settings::domainSize[0];

					positionInHalo[0] = realDomainSize + p.x[0];
					positionInHalo[1] = p.x[1];
					positionInHalo[2] = p.x[2];
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[0] + Settings::domainSize[0];
					container.add(pNew);

					if(p.x[1] < Settings::rCutoff){
						pNew.x[1] += Settings::domainSize[1];
						container.add(pNew);
						if(p.x[2] < Settings::rCutoff){
							pNew.x[2] += Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
							pNew.x[2] -= Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;

						}
					}else if(p.x[1] > Settings::domainSize[1] - Settings::rCutoff){
						pNew.x[1] -= Settings::domainSize[1];
						container.add(pNew);
						if(p.x[2] < Settings::rCutoff){
							pNew.x[2] += Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
							pNew.x[2] -= Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}
					}

					if(p.x[2] < Settings::rCutoff){
						pNew.x[2] += Settings::domainSize[2];
						container.add(pNew);
					}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
						pNew.x[2] -= Settings::domainSize[2];
						container.add(pNew);
				    }
					return false;


				},
				//x0 = domain[0] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = Settings::domainSize[0];

					positionInHalo[0] = p.x[0] - realDomainSize;
					positionInHalo[1] = p.x[1];
					positionInHalo[2] = p.x[2];
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[0] - Settings::domainSize[0];

					container.add(pNew);

					if(p.x[1] < Settings::rCutoff){
						pNew.x[1] += Settings::domainSize[1];
						container.add(pNew);
						if(p.x[2] < Settings::rCutoff){
							pNew.x[2] += Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
							pNew.x[2] -= Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}
					}else if(p.x[1] > Settings::domainSize[1] - Settings::rCutoff){
						pNew.x[1] -= Settings::domainSize[1];
						container.add(pNew);
						if(p.x[2] < Settings::rCutoff){
							pNew.x[2] += Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
							pNew.x[2] -= Settings::domainSize[2];
							container.add(pNew);
							pNew.x= positionInHalo;
						}
					}

					if(p.x[2] < Settings::rCutoff){
						pNew.x[2] += Settings::domainSize[2];
						container.add(pNew);
					}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
						pNew.x[2] -= Settings::domainSize[2];
						container.add(pNew);
				    }

					return false;
				},
				//x1 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = Settings::domainSize[1];

					positionInHalo[0] = p.x[0];
					positionInHalo[1] = p.x[1] + realDomainSize;
					positionInHalo[2] = p.x[2];
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[1] + Settings::domainSize[1];
					container.add(pNew);

					/*if(p.x[0] < Settings::rCutoff){
						pNew.x[0] += Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}else if(p.x[0] > Settings::domainSize[0] - Settings::rCutoff){
						pNew.x[0] -= Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}*/

					if(p.x[2] < Settings::rCutoff){
						pNew.x[2] += Settings::domainSize[2];
						container.add(pNew);
					}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
						pNew.x[2] -= Settings::domainSize[2];
						container.add(pNew);
					}
					return false;
				},
				//x1 = domain[1] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = Settings::domainSize[1];

					positionInHalo[0] = p.x[0];
					positionInHalo[1] = p.x[1] - realDomainSize;
					positionInHalo[2] = p.x[2];
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[1] - Settings::domainSize[1];
					container.add(pNew);

					/*if(p.x[0] < Settings::rCutoff){
						pNew.x[0] += Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}else if(p.x[0] > Settings::domainSize[0] - Settings::rCutoff){
						pNew.x[0] -= Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}*/

					if(p.x[2] < Settings::rCutoff){
						pNew.x[2] += Settings::domainSize[2];
						container.add(pNew);
					}else if(p.x[2] > Settings::domainSize[2] - Settings::rCutoff){
						pNew.x[2] -= Settings::domainSize[2];
						container.add(pNew);
					}

					return false;
				},
				//x2 = 0 boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;
					double realDomainSize = Settings::domainSize[2];

					positionInHalo[0] = p.x[0];
					positionInHalo[1] = p.x[1];
					positionInHalo[2] = realDomainSize + p.x[2];
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[2] + Settings::domainSize[2];
					container.add(pNew);

					/*if(p.x[0] < Settings::rCutoff){
						pNew.x[0] += Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}else if(p.x[0] > Settings::domainSize[0] - Settings::rCutoff){
						pNew.x[0] -= Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}*/

					/*if(p.x[1] < Settings::rCutoff){
						pNew.x[1] += Settings::domainSize[1];
						container.add(pNew);
					}else if(p.x[1] > Settings::domainSize[1] - Settings::rCutoff){
						pNew.x[1] -= Settings::domainSize[1];
						container.add(pNew);
					}*/

					return false;
				},
				//x2 = domain[2] boundary
				[] (ParticleContainer &container, Particle &p) {
					utils::Vector <double ,3> positionInHalo;
					utils::Vector <double, 3> zeroVelocity;

					double realDomainSize = Settings::domainSize[2];
					positionInHalo[0] = p.x[0];
					positionInHalo[1] = p.x[1];
					positionInHalo[2] = p.x[2] - realDomainSize;
					Particle pNew (positionInHalo, p.v, p.type, p.id);
					pNew.x_t0 = p.x_t0[2] - Settings::domainSize[2];
					container.add(pNew);

					/*if(p.x[0] < Settings::rCutoff){
						pNew.x[0] += Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}else if(p.x[0] > Settings::domainSize[0] - Settings::rCutoff){
						pNew.x[0] -= Settings::domainSize[0];
						container.add(pNew);
						pNew.x[0] = positionInHalo[0];
					}*/

					/*if(p.x[1] < Settings::rCutoff){
						pNew.x[1] += Settings::domainSize[1];
						container.add(pNew);
					}else if(p.x[1] > Settings::domainSize[1] - Settings::rCutoff){
						pNew.x[1] -= Settings::domainSize[1];
						container.add(pNew);
					}*/
					return false;
				} };

SimulationScenario *ScenarioFactory::build(ScenarioType type) {
	SimulationScenario *scenario = new SimulationScenario;

	scenario->addAdditionalForces = ScenarioFactory::addAdditionalForces;

	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON)
		scenario->updatePosition = ScenarioFactory::verletUpdatePositionThermostate;
	else
		scenario->updatePosition = ScenarioFactory::verletUpdatePosition;

	if (type == ScenarioType::Gravity) {

		scenario->calculateForce = ScenarioFactory::calculateGravityForce;

		scenario->setup = ScenarioFactory::basicFileReaderSetup;

		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;
		LOG4CXX_DEBUG(logger, "Built Gravity Scenario");
	} else if (type == ScenarioType::Lennard_Jones) {
		scenario->calculateForce =
				ScenarioFactory::calculateLennardJonesPotentialForce;

		scenario->setup = ScenarioFactory::LennardJonesSetup;

		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;

		LOG4CXX_DEBUG(logger, "Built Lennard-Jones Scenario");
	} else if (type == ScenarioType::Membrane) {
		scenario->calculateForce = ScenarioFactory::calculateMembraneForce;

		scenario->setup = ScenarioFactory::LennardJonesSetup;

		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;
	} else if (type == ScenarioType::Lennard_Jones_Smoothed) {
		scenario->calculateForce = ScenarioFactory::calculateSmoothLJ;

		scenario->setup = ScenarioFactory::LennardJonesSetup;

		scenario->updateVelocity = ScenarioFactory::verletUpdateVelocity;
	}else {
		LOG4CXX_FATAL(logger, "Unknown Simulation type!");
		exit(-1);
	}

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
							calcForce(p, phantom);
						}
						return false;
					},
					//x0 = domain[0] boundary, "right"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = Settings::domainSize[0];
						if(p.x[0] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = realDomainSize;
							phantom.x[1] = p.x[1];
							phantom.x[2] = p.x[2];
							calcForce(p, phantom);
						}
						return false;
					},
					//x1 = 0 boundary, "bottom"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						if(p.x[1] < TWORAISED1_6 * Settings::particleTypes[p.type].sigma) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = 0;
							phantom.x[2] = p.x[2];
							calcForce(p, phantom);
						}
						return false;
					},
					//x1 = domain[1] boundary, "top"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = Settings::domainSize[1];
						if(p.x[1] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = realDomainSize;
							phantom.x[2] = p.x[2];
							calcForce(p, phantom);
						}
						return false;
					},
					//x2 = 0 boundary, "back"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						if(p.x[2] < TWORAISED1_6 * Settings::particleTypes[p.type].sigma) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = p.x[1];
							phantom.x[2] = 0;
							calcForce(p, phantom);
						}
						return false;
					},
					//x2 = domain[2] boundary, "front"
					[&calcForce, &phantom] (ParticleContainer &container, Particle &p) {
						double realDomainSize = Settings::domainSize[2];
						if(p.x[2] > (realDomainSize - TWORAISED1_6 * Settings::particleTypes[p.type].sigma)) {
							phantom.x[0] = p.x[0];
							phantom.x[1] = p.x[1];
							phantom.x[2] = realDomainSize;
							calcForce(p, phantom);
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
