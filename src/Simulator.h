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

class Simulator {
private:
	std::list<Particle> particles;

public:
	Simulator();
	virtual ~Simulator();

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
	 * reads the particle configuration input file
	 * @param inputFile the path to the input configuration file
	 */
	void readInputFile(char* inputFile);

	/**
	 * advances the simulation by a time step of Settings::deltaT
	 */
	void nextTimeStep();

};

#endif /* SIMULATOR_H_ */
