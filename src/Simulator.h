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
#include "CellListContainer.h"
#include "ParticleContainer.h"
#include "ScenarioFactory.h"
#include "utils/Thermostat.h"
#include <log4cxx/logger.h>

class Simulator {
private:

	SimulationScenario *scenario;
	Thermostat *thermostat;
	ParticleContainer *particleContainer;
public:
	Simulator();
	virtual ~Simulator();
	static log4cxx::LoggerPtr logger;

	void exportPhaseSpace(void);


	/* processed iterations*/
	static int iterations;

	/**
	 * calculate the force for all particles
	 */
	void calculateF();

	/**
	 * calculate the position for all particles
	 */
	void calculateX();

	/**
	 * calculate the velocity for all particles
	 */
	void calculateV();

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
