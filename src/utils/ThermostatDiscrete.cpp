/*
 * ThermostatDiscrete.cpp
 *
 *  Created on: 24.01.2013
 *      Author: Leo
 */

#include "ThermostatDiscrete.h"
#include "Simulator.h"
#include "MaxwellBoltzmannDistribution.h"
#include "Settings.h"


double ThermostatDiscrete::beta = 1;
double ThermostatDiscrete::currentEnergy = 0 ;
double ThermostatDiscrete::temperaturePerStep = 0;
double ThermostatDiscrete::numberOfParticles = 0;
double ThermostatDiscrete::currentTemperature = 0;
double ThermostatDiscrete::targetTemperature = 0;
double ThermostatDiscrete::initialTemperature = 0;
int ThermostatDiscrete::controlInterval = 1000;
int ThermostatDiscrete::dimensions = 3;

log4cxx::LoggerPtr ThermostatDiscrete::logger = log4cxx::Logger::getLogger("ThermostatDiscrete");

void ThermostatDiscrete::initialize(int dimensions, int numberOfParticles, ParticleContainer *container) {
	controlInterval = Settings::thermostatSettings->controlInterval();
	ThermostatDiscrete::dimensions = dimensions;
	ThermostatDiscrete::numberOfParticles = numberOfParticles;


	setInitialTemperature(container);

	//set up target temperature
	if(Settings::thermostatSettings->targetTemperature().present()) {
		targetTemperature = Settings::thermostatSettings->targetTemperature().get();
	}
	//if not present, set to initial temperature to hold it
	else
		targetTemperature = initialTemperature;


	if(Settings::thermostatSettings->temperaturePerStep().present()) {
		temperaturePerStep = Settings::thermostatSettings->temperaturePerStep().get();
	}
	else {
		temperaturePerStep = 0;
	}

	currentTemperature = initialTemperature;

	//logger->setLevel(log4cxx::Level::getTrace());
}

void ThermostatDiscrete::setInitialTemperature(ParticleContainer *container) {
	auto initialTemperature_arg = Settings::thermostatSettings->initTemperature();

	if(initialTemperature_arg.present()){
		initialTemperature = initialTemperature_arg.get();

		container->each([&] (Particle& p) {
			double scale = sqrt( BOLTZMANN * initialTemperature / (Settings::particleTypes[p.type].mass));
			MaxwellBoltzmannDistribution(p , scale , Settings::dimensions);
		});

		calculateCurrentEnergy(container);
		scaleVelocities(container);

		LOG4CXX_DEBUG(logger,"Applied initial Temperature " << initialTemperature_arg.get());
		LOG4CXX_DEBUG(logger,"Initial energy should be \t"<< initialTemperature * BOLTZMANN * numberOfParticles * dimensions / 2 );

	}else{
		//if no initial temperature given, determine current temperature
		currentEnergy = 0;
		container->each([&] (Particle& p) {
			currentEnergy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared();
		});
		currentEnergy = currentEnergy / 2;
		initialTemperature = 2 * currentEnergy / (dimensions * numberOfParticles * BOLTZMANN);

		LOG4CXX_DEBUG(logger,"Initial Temperature not given, current temperature is " << initialTemperature);
	}
}

void ThermostatDiscrete::scaleVelocities(ParticleContainer *p) {
	p->each([](Particle &p) {
		p.v = beta * p.v;
	});
	LOG4CXX_TRACE(logger, "Scaling velocities by " << beta);
}

void ThermostatDiscrete::calculateCurrentEnergy(ParticleContainer * particles){

	numberOfParticles = particles->getSize();

	currentEnergy = 0;


	particles->each([&] (Particle& p) {
		currentEnergy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared();
	});

	currentEnergy = currentEnergy / 2;

	currentTemperature = 2 * currentEnergy / (dimensions * numberOfParticles * BOLTZMANN);

	LOG4CXX_TRACE(logger,"Temperature : " << currentTemperature << " degrees; Energy is :"<<ThermostatDiscrete::currentEnergy <<" after: " << Simulator::iterations << " iterations; " << numberOfParticles << " particles");


	/*if(energyDif == 0){
		beta = 1;
	}else if(energyDif > 0){
		if(energyDif * energyDif >= energyPerStep * energyPerStep){
			beta = sqrt(1 + (energyPerStep) / currentEnergy);
		} else {
			beta = sqrt(1 + (energyDif / currentEnergy));
		}
	LOG4CXX_TRACE(logger,"ScalingVelocity by :\t" << beta << "; current energy is " << currentEnergy << " EnergyDifference is:" << energyDif);
	}else{
		if(energyDif * energyDif >= energyPerStep * energyPerStep){
		beta = sqrt(1-(energyPerStep)/currentEnergy);
		}else{
		beta = sqrt(1+(energyDif/currentEnergy));
		}
	LOG4CXX_TRACE(logger,"ScalingVelocity by :\t" << beta << "; current energy is " << currentEnergy << " EnergyDifference is:" << energyDif);
	}*/


	double stepTargetTemp = targetTemperature;

	//adjust target energy so maximum energy per step is not exceeded
	//do this only if we have not reached our target temperature yet, otherwise, we perform direct downscaling, ignoring ther maximum temperature per step
	if(temperaturePerStep > 0) {

		double tempDif = targetTemperature - currentTemperature;
		LOG4CXX_TRACE(logger, "We have a maximum temperature per step: " << temperaturePerStep << "; target temperature is " << stepTargetTemp);

		if(tempDif > temperaturePerStep)
			stepTargetTemp = currentTemperature + temperaturePerStep;
		else if(tempDif < -temperaturePerStep)
			stepTargetTemp = currentTemperature - temperaturePerStep;

		LOG4CXX_TRACE(logger, "New target temperature is " << stepTargetTemp);

		if(sgn(targetTemperature - initialTemperature) != sgn(targetTemperature - currentTemperature)) {
			temperaturePerStep = 0.0;
			std::cout << "*******************************Temperature reached!*********************************" << std::endl;
		}
	}

	beta = sqrt(stepTargetTemp/currentTemperature);

	if(currentEnergy == 0){
			beta = 1;
			LOG4CXX_FATAL(logger,"NO ENERGY something must have gone wrong !");
	}

}

void ThermostatDiscrete::updateThermostate(ParticleContainer * particles){
		calculateCurrentEnergy(particles);
}

double ThermostatDiscrete::getTemperature(ParticleContainer * container) {
	double energy = 0;

	container->each([&] (Particle& p) {
		energy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared();
	});


	return energy / (dimensions * container->getSize() * BOLTZMANN);
}
