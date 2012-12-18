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

	void getStepEnergy();
	void initTargetEnergy();
	void calculateCurrentEnergy(ParticleContainer* particles);
	void iterateBeta();
	void setCurrentTemperature(ParticleContainer* particles);


	double targetEnergy;
	double energyPerStep;
	double beta;
	int dimensions;
	int numberOfParticles;

public:

	Thermostat(int arg_dimensions , int arg_numberOfParticles);
	~Thermostat();

	void thermostate(ParticleContainer *particles);
	void scaleInitialVelocity(ParticleContainer *particles);

	static double currentEnergy;
	static double currentTemperature;
	static log4cxx::LoggerPtr logger;

};


#endif /* THERMOSTAT_H_ */
