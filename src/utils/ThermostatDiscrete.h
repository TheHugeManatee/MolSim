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
	ThermostatDiscrete();
	static void updateThermostate(ParticleContainer *particles);

	static double beta;
	static double currentEnergy;
	static double energyPerStep;
	static double numberOfParticles;
	static double currentTemperature;
	static double targetEnergy;

private:
	static log4cxx::LoggerPtr logger;

	/**
	 * Calculates and returns the energy that has to be applied to the system in one timestep
	 * accoring to the remaining steps and remaining Energy to be applied
	 * @return delta E_kin
	 */
	static void getStepEnergy();

	/**
	 * Calculates the current energy for the given ParticleContainer
	 * and stores it in the static Thermostat::currentEnergy field
	 * the numberOfParticles Value is reseted
	 * also beta is initialized according to current Energy
	 *
	 * @param particles ParticleContainer of which the current Energy is calculated
	 */
	static void calculateCurrentEnergy(ParticleContainer* particles);

	/**
	 * Calculates current energy
	 */
	static void setCurrentTemperature();

};

#endif /* THERMOSTATDISCRETE_H_ */
