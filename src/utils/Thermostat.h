/*
 * Thermostat.h
 *
 *  Created on: 15.12.2012
 *      Author: Leo
 */

#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_

#include "ParticleContainer.h"
#include <utils/Settings.h>

//#define BOLTZMANN 0.000000000000000000000013806503
#define BOLTZMANN 1

class Thermostat{

private:
	/**
	 * Calculates and returns the energy that has to be applied to the system in one timestep
	 * accoring to the remaining steps and remaining Energy to be applied
	 * @return delta E_kin
	 */
	static void getStepEnergy();

	/**
	 * Calculates and returns the energy of the temperature to be reached
	 * @return The target energy
	 */
	static void initTargetEnergy();

	/**
	 * calculates beta for the next timestep from the previous beta
	 */
	static void iterateBeta();

	/**
	 * Calculates the current energy for the given ParticleContainer
	 * and stores it in the static Thermostat::currentEnergy field
	 * the numberOfParticles Value is reseted
	 * also beta is initialized according to current Energy
	 *
	 * @param particles ParticleContainer of which the current Energy is calculated
	 */
	static void calculateCurrentEnergy(ParticleContainer* particles);

	static double targetEnergy;
	static double energyPerStep;
	static int dimensions;
	static int numberOfParticles;

	static int maxSteps;
	static int stepSize;

public:

	static void initialize(int arg_dimensions , int arg_numberOfParticles);


	/*all the thermostation work*/
	static void updateThermostate(ParticleContainer *particles);

	/**
	 * Scales the particles of the given particleGenerator by the initial Temperature
	 */
	static void scaleInitialVelocity(ParticleContainer *particles);


	static void setCurrentTemperature(ParticleContainer* particles);

	static double currentEnergy;
	static double currentTemperature;
	static log4cxx::LoggerPtr logger;
	static double beta;

};


#endif /* THERMOSTAT_H_ */
