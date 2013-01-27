/**
 * @file Simulator.h
 *
 * a simulator offering methods to calculate force, postition and velocitiy for a given list of particles.
 *
 * @date Oct 30, 2012
 * @author j
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <list>

#include "Particle.h"
#include "ParticleContainer.h"
#include "ScenarioFactory.h"
#include "utils/Thermostat.h"
#include "utils/ThermostatDiscrete.h"
#include "JobQueue.h"
#include <log4cxx/logger.h>
#include <sstream>
#include <iostream>

#define PI 3.1415926535897932384626433;

class Simulator {
private:

	SimulationScenario *scenario;
	ParticleContainer *particleContainer;

#ifdef _OPENMP
	JobQueue *simulationJobs;
	JobQueue *apcJobs;
#endif

public:
	Simulator();
	virtual ~Simulator();
	static log4cxx::LoggerPtr logger;

	static double diffusion;
	static double* radialDistribution;
	static std::stringstream statistics;

	void exportPhaseSpace(void);


	void getDiffusion();
	void getRadialDistribution();
	void addStatisticsString();
	void exportStatistics();


	/* processed iterations*/
	static int iterations;

	/**
	 * applies given force fields and gravity
	 */

	void addAdditionalForces();

	/**
	 * plot the particles to a xyz-file
	 * @param iteration the number of the iteration, for naming purposes
	 */
	void plotParticles(int iteration);

	/**
	 * advances the simulation by a time step of Settings::deltaT
	 */
	void nextTimeStep();


};

#endif /* SIMULATOR_H_ */
