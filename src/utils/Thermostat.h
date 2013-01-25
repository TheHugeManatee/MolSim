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

//we're always scaling with 10^(21) (heuristically determined ;-))
#define BOLTZMANN 1.3806503

class Thermostat{

private:

	static void getStepEnergy();
	static void initTargetEnergy();
	static void iterateBeta();
	static void calculateCurrentEnergy(ParticleContainer* particles);

	static double targetEnergy;
	static double energyPerStep;
	static int dimensions;
	static int numberOfParticles;

	static int maxSteps;
	static int stepSize;

public:

	static void initialize(int arg_dimensions , int arg_numberOfParticles);

	static void updateThermostate(ParticleContainer *particles);
	static void scaleInitialVelocity(ParticleContainer *particles);
	static void setCurrentTemperature(ParticleContainer* particles);

	static double currentEnergy;
	static double currentTemperature;
	static log4cxx::LoggerPtr logger;
	static double beta;

};


#endif /* THERMOSTAT_H_ */
