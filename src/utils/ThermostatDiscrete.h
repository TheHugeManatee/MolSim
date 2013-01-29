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
	static void getStepEnergy();
	static void calculateCurrentEnergy(ParticleContainer* particles);
	static void setCurrentTemperature();

};

#endif /* THERMOSTATDISCRETE_H_ */
