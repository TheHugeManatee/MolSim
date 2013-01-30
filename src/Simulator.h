/**
 * @file Simulator.h
 *
 *
 * @date Oct 30, 2012
 * @author Jakob Weiss
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

/**
 * @class Simulator
 *  a simulator offering methods to calculate force, position and velocity for a given list of particles.
 */

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

	/**
	 * exports all important information of every particle in the simulation domain to a Text File
	 * that can be reimported in order to resume a simulation either with the same other settings
	 * (like gravitation or boundary conditions) or different settings for example to first equilibriate
	 * a liquid and then throw something into it.
	 *
	 */
	void exportPhaseSpace(void);

	/**
	 * Implements the calculation of diffusion for thermodynamical statistics
	 */
	void getDiffusion();

	/**
	 * Implements the calculation of the Radial Distribution Function (RDF) for thermodynamical statistics
	 */
	void getRadialDistribution();

	/**
	 * Writes all statistic data to the statistics variable
	 */
	void addStatisticsString();

	/**
	 * Writes all statistic data to a file that is defined in Settings::statisticsFile
	 */
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
