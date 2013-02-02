/*
 * ThermostatDiscrete.h
 *
 *  Created on: 24.01.2013
 *      Author: Leo
 */

#ifndef THERMOSTATDISCRETE_H_
#define THERMOSTATDISCRETE_H_
#include "ParticleContainer.h"
#include <utils/Settings.h>

#define BOLTZMANN 1

/**
 * @class ThermostatDiscrete
 *
 * This class implements a discrete Thermostat that changes the velocities according to a temperature that is
 * to be reached or held. In contrast to the class Thermostat it works better for larger differences in the
 * temperatures but as it is discrete it produces bigger discontinuities
 *
 */
class ThermostatDiscrete {
public:
	static void updateThermostate(ParticleContainer *particles);

	static void initialize(int dimensions, int numberOfParticles, ParticleContainer *container);

	static void scaleVelocities(ParticleContainer *container);

	static double getTemperature(ParticleContainer *container);

	static double beta;
	static double currentEnergy;
	static double temperaturePerStep;
	static double numberOfParticles;
	static double currentTemperature;
	static double targetTemperature;
	static double initialTemperature;
	static int dimensions;
	static int controlInterval;

private:
	static log4cxx::LoggerPtr logger;

	static void setInitialTemperature(ParticleContainer *container);


	/**
	 * Calculates the current energy for the given ParticleContainer
	 * and stores it in the static Thermostat::currentEnergy field
	 * the numberOfParticles Value is reseted
	 * also beta is initialized according to current Energy
	 *
	 * @param particles ParticleContainer of which the current Energy is calculated
	 */
	static void calculateCurrentEnergy(ParticleContainer* particles);


};

#endif /* THERMOSTATDISCRETE_H_ */
