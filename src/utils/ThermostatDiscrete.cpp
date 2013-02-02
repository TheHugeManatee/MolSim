/*
 * ThermostatDiscrete.cpp
 *
 *  Created on: 24.01.2013
 *      Author: Leo
 */

#include "ThermostatDiscrete.h"
#include "Simulator.h"


double ThermostatDiscrete::beta = 1;
double ThermostatDiscrete::currentEnergy = 0 ;
double ThermostatDiscrete::energyPerStep = 0;
double ThermostatDiscrete::numberOfParticles = 0;
double ThermostatDiscrete::currentTemperature = 0;
double ThermostatDiscrete::targetEnergy=0;
int ThermostatDiscrete::controlInterval = 1000;
log4cxx::LoggerPtr ThermostatDiscrete::logger = log4cxx::Logger::getLogger("ThermostatDiscrete");

void ThermostatDiscrete::initialize(int dimensions, int numberOfParticles) {
	controlInterval = Settings::thermostatSettings->controlInterval();
}

void ThermostatDiscrete::scaleVelocities(ParticleContainer *p) {
	p->each([](Particle &p) {
		p.v = beta * p.v;
	});
}

void ThermostatDiscrete::getStepEnergy(){
	auto temperaturePerStep_opt = Settings::thermostatSettings->temperaturePerStep();
	if(temperaturePerStep_opt.present()){
	//	std::cout << "numberOfParticles: " << numberOfParticles << std::endl;
	//	std::cout << "temperaturePerStep: " << temperaturePerStep << std::endl;
		energyPerStep = Settings::dimensions / 2.0 * numberOfParticles * (temperaturePerStep_opt.get()) * BOLTZMANN;
	//	std::cout << "energyPerStep: " << energyPerStep << std::endl;
	}else{
		energyPerStep = targetEnergy -currentEnergy;
	//	LOG4CXX_DEBUG(logger,"Target Energy: " << targetEnergy <<" Current Energy: " <<currentEnergy);

	}

}

void ThermostatDiscrete::calculateCurrentEnergy(ParticleContainer * particles){

	numberOfParticles = particles->getSize();
	targetEnergy = Settings::dimensions / 2.0 * numberOfParticles * (Settings::thermostatSettings->targetTemperature().get()) * BOLTZMANN;
	ThermostatDiscrete::currentEnergy = 0 ;

	particles->each([&] (Particle& p) {
		ThermostatDiscrete::currentEnergy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared() ;
	});

	ThermostatDiscrete::currentEnergy = ThermostatDiscrete::currentEnergy / 2;

	ThermostatDiscrete::setCurrentTemperature();

	LOG4CXX_TRACE(logger,"Current Energy is :\t"<<ThermostatDiscrete::currentEnergy <<"\t after: " << Simulator::iterations << "\t iterations");

	//	initTargetEnergy();  //has to be initialized again if number of particles changes !!
	getStepEnergy();

	LOG4CXX_TRACE(logger, "Currently there are " << numberOfParticles << " particles.");
	double energyDif = targetEnergy - currentEnergy;

	if(energyDif == 0){
		beta = 1;
	}else if(energyDif > 0){
		if(energyDif * energyDif >= energyPerStep * energyPerStep){
			beta = sqrt(1+(energyPerStep)/currentEnergy);
		}else{
			beta = sqrt(1+(energyDif/currentEnergy));
		}
	LOG4CXX_TRACE(logger,"ScalingVelocity by :\t" << beta << "; current energy is " << currentEnergy << " EnergyDifference is:" << energyDif);
	}else{
		if(energyDif * energyDif >= energyPerStep * energyPerStep){
		beta = sqrt(1-(energyPerStep)/currentEnergy);
		}else{
		beta = sqrt(1+(energyDif/currentEnergy));
		}
	LOG4CXX_TRACE(logger,"ScalingVelocity by :\t" << beta << "; current energy is " << currentEnergy << " EnergyDifference is:" << energyDif);
	}

	if(currentEnergy == 0){
			beta = 1;
			LOG4CXX_FATAL(logger,"NO ENERGY something must have gone wrong !");
	}
}

void ThermostatDiscrete::setCurrentTemperature(){
	double currentTemperature_arg = 2 * currentEnergy / (Settings::dimensions * numberOfParticles * BOLTZMANN);
	currentTemperature = currentTemperature_arg;
	LOG4CXX_TRACE(logger,"Temperature : " << currentTemperature << " degrees");
}

void ThermostatDiscrete::updateThermostate(ParticleContainer * particles){
	if(Simulator::iterations % Settings::thermostatSettings->controlInterval() == 0 ){
		calculateCurrentEnergy(particles);
	}

}
