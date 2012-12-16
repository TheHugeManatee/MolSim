/*
 * Thermostat.cpp
 *
 *  Created on: 15.12.2012
 *      Author: Leo
 */

#include <utils/Thermostat.h>
#include <Simulator.h>
#include <utils/MaxwellBoltzmannDistribution.h>


log4cxx::LoggerPtr Thermostat::logger = log4cxx::Logger::getLogger("Thermostat");

double Thermostat::currentEnergy = 0;
double Thermostat::currentTemperature = 0;

Thermostat::Thermostat(int arg_dimensions , int arg_numberOfParticles){

	LOG4CXX_INFO(logger,"Thermostat turned on: Will check every "<< Settings::thermostatSettings->controlInterval()<<" steps");
	LOG4CXX_INFO(logger,"Goal are "<< Settings::thermostatSettings->targetTemperature() << " degrees");
	numberOfParticles = arg_numberOfParticles;
	std::cout << "Number of Particles :\t" << numberOfParticles << std::endl;
	dimensions = arg_dimensions;
	initTargetEnergy();
	getStepEnergy();
	beta = sqrt(1+energyPerStep/targetEnergy);

}

Thermostat::~Thermostat(){
}

/**
 * Calculates and returns the energy of the temperature to be reached
 * @return The target energy
 */
void Thermostat::initTargetEnergy(){
	double targetTemperature = Settings::thermostatSettings->targetTemperature() ;
//	std::cout << dimensions <<"\t" << numberOfParticles << "\t" << targetTemperature << "\t" << boltzmann << std::endl;
	double arg_targetEnergy =  dimensions / 2.0 * numberOfParticles * (targetTemperature + 273.2) * boltzmann; //transfer from celsius to kelvin !

	LOG4CXX_DEBUG(logger,"Target Energy is :\t"<<arg_targetEnergy);
	Thermostat::targetEnergy = arg_targetEnergy;
}

/**
 * Calculates and returns the energy that has to be applied to the system in one timestep
 * accoring to the left steps and remaining Energy to be applied
 * @return delta E_kin
 */
void Thermostat::getStepEnergy(){
	int stepsDone = Simulator::iterations;
	int stepsLeft = Settings::thermostatSettings->estimatedSteps() - stepsDone;
	double deltaEnergy = targetEnergy-currentEnergy;
	energyPerStep = deltaEnergy / stepsLeft ;
	if(energyPerStep <= 0){
		LOG4CXX_DEBUG(logger,"Cooling down.");
	}else{
		LOG4CXX_DEBUG(logger,"Heating up.");
	}
}

/**
 * Scales the particles of the given particleGenerator by the initial Temperature
 */
void Thermostat::scaleInitialVelocity(ParticleContainer *particles){
	auto initialTemperature = Settings::thermostatSettings->initTemperature();
	if(initialTemperature.present()){
	particles->each([&] (Particle& p) {
		double scale = sqrt( boltzmann *initialTemperature.get()/p.m ) ;
		MaxwellBoltzmannDistribution(p , scale , 3);
	});

	LOG4CXX_DEBUG(logger,"Applied initial Temperature");
	}else{
	LOG4CXX_DEBUG(logger,"Initial Temperature not given, continuing without further velocity scaling");
	}
}

/**
 * Calculates the current energy for the given ParticleContainer
 * and stores it in the static Thermostat::currentEnergy field
 * the numberOfParticles Value is reseted
 * also beta is initialized according to current Energy
 */

void Thermostat::calculateCurrentEnergy(ParticleContainer* particles){
	Thermostat::currentEnergy = 0 ;
	particles->each([&] (Particle& p) {
			Thermostat::currentEnergy += p.m * p.v.LengthOptimizedR3Squared() ;
	});

	Thermostat::currentEnergy = Thermostat::currentEnergy / 2;

	setCurrentTemperature(particles);

	LOG4CXX_DEBUG(logger,"Actual Energy is :\t"<<Thermostat::currentEnergy);

	numberOfParticles = particles->getSize();

	initTargetEnergy();  //has to be initialized again because number of particles could have changed !!
	getStepEnergy();
	beta = sqrt(1+energyPerStep/Thermostat::currentEnergy);
}


/**
 * calculates beta for the actual timestep from the previous beta
 */
void Thermostat::iterateBeta(){
	beta = sqrt(2-1/(beta*beta)); /*Ask Leo if you want to know why that works he can give you a wonderful prove*/
}

/*all the thermostation work*/
void Thermostat::thermostate(ParticleContainer* particles){
	iterateBeta();
	int iterations = Simulator::iterations;
	int stepSize = Settings::thermostatSettings->controlInterval();
	if((iterations %  stepSize) == 0 ){
		calculateCurrentEnergy(particles);
	}
	particles->each([&] (Particle& p) {
		p.v = p.v * beta;
	});

}

void Thermostat::setCurrentTemperature(ParticleContainer* particles){
	double currentTemperature_arg = 2 * currentEnergy / (3 * particles->getSize() * boltzmann) - 273.2;
	currentTemperature = currentTemperature_arg;
	LOG4CXX_DEBUG(logger,"Temperature : " << Thermostat::currentTemperature << " degrees");
}

