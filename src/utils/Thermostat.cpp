/*
 * Thermostat.cpp
 *
 *  Created on: 15.12.2012
 *      Author: Leo
 */

#include "utils/Thermostat.h"
#include "Simulator.h"
#include "utils/MaxwellBoltzmannDistribution.h"

log4cxx::LoggerPtr Thermostat::logger = log4cxx::Logger::getLogger("Thermostat");

double Thermostat::currentEnergy = 0;
double Thermostat::currentTemperature = 0;
double Thermostat::beta = 1;
double Thermostat::targetEnergy;
double Thermostat::energyPerStep;
int Thermostat::dimensions;
int Thermostat::numberOfParticles;
int Thermostat::maxSteps;
int Thermostat::stepSize;

void Thermostat::initialize(int arg_dimensions , int arg_numberOfParticles){
	stepSize = Settings::thermostatSettings->controlInterval();
	LOG4CXX_INFO(logger,"Thermostat turned on: Will check every "<< stepSize <<" steps");
	auto targetTemperature_opt = Settings::thermostatSettings->targetTemperature();
	if(targetTemperature_opt.present()){
		LOG4CXX_INFO(logger,"Goal are "<< Settings::thermostatSettings->targetTemperature() << " degrees");
	}else{
		LOG4CXX_INFO(logger,"Thermostat is trying to the keep temperature");
	}
	numberOfParticles = arg_numberOfParticles;
	if(numberOfParticles == 0)
		LOG4CXX_FATAL(logger,"No particles! Result will be wrong .... !");

	dimensions = arg_dimensions;

	auto maxSteps_opt = Settings::thermostatSettings->maxSteps();
	if (maxSteps_opt.present()) {
		maxSteps = maxSteps_opt.get();
		LOG4CXX_INFO(logger,"Thermostat is applied for "<< maxSteps <<" iterations");

	}else{
		maxSteps=0;
	}

	initTargetEnergy();

}


void Thermostat::scaleInitialVelocity(ParticleContainer *particles){
	auto initialTemperature_arg = Settings::thermostatSettings->initTemperature() ;
	numberOfParticles = particles->getSize();
	if(initialTemperature_arg.present()){
		double initialTemperature = initialTemperature_arg.get();
		particles->each([&] (Particle& p) {
			double scale = sqrt( BOLTZMANN * initialTemperature / (Settings::particleTypes[p.type].mass));
			MaxwellBoltzmannDistribution(p , scale , Settings::dimensions);
		});

		LOG4CXX_DEBUG(logger,"Applied initial Temperature " << initialTemperature_arg.get());
		LOG4CXX_DEBUG(logger,"Initial energy should be \t"<< initialTemperature * BOLTZMANN * numberOfParticles * Settings::dimensions / 2 );

	}else{
		LOG4CXX_DEBUG(logger,"Initial Temperature not given, continuing without further velocity scaling");
	}

//	calculateCurrentEnergy(particles);
/**	particles->each([&] (Particle& p) {
		Thermostat::currentEnergy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared() ;
	});
	Thermostat::currentEnergy = Thermostat::currentEnergy / 2;
**/
//	LOG4CXX_DEBUG(logger,"Initial energy is \t"<< Thermostat::currentEnergy);
}


void Thermostat::initTargetEnergy(){
	auto targetTemperature_opt = Settings::thermostatSettings->targetTemperature() ;
	auto initialTemperature_opt = Settings::thermostatSettings->initTemperature();
	if(targetTemperature_opt.present()){
		double targetTemperature = targetTemperature_opt.get();
		//	std::cout << dimensions <<"\t" << numberOfParticles << "\t" << targetTemperature << "\t" << boltzmann << std::endl;
		double arg_targetEnergy = dimensions / 2.0 * numberOfParticles * (targetTemperature) * BOLTZMANN;   //transfer from celsius to kelvin !
		LOG4CXX_DEBUG(logger,"Target Energy is :\t"<<arg_targetEnergy);
		Thermostat::targetEnergy = arg_targetEnergy;
	}else if(initialTemperature_opt.present()){
		double initTemperature = initialTemperature_opt.get();
		double arg_initialEnergy = 	dimensions / 2.0 * numberOfParticles * (initTemperature) * BOLTZMANN;
		Thermostat::targetEnergy = arg_initialEnergy ;
		LOG4CXX_DEBUG(logger,"Target Energy is :\t"<<arg_initialEnergy);
	}else{
		Thermostat::targetEnergy = Thermostat::currentEnergy;
	}
}


void Thermostat::getStepEnergy(){
	auto estimatedSteps_opt = Settings::thermostatSettings->estimatedSteps();
	int stepsLeft;
	if(estimatedSteps_opt.present()){
		int stepsDone = Simulator::iterations;
		stepsLeft = Settings::thermostatSettings->estimatedSteps().get() - stepsDone;
	}else{
		stepsLeft = stepSize;
	}

	if(stepsLeft <= 0){
		stepsLeft = stepSize;
	}

	double deltaEnergy = targetEnergy-currentEnergy;

	energyPerStep = deltaEnergy / (stepsLeft + 1);

	if(energyPerStep <= 0){
		LOG4CXX_TRACE(logger,"Cooling down. "<<deltaEnergy);
	}else{
		LOG4CXX_TRACE(logger,"Heating up."<<deltaEnergy);
	}
}



void Thermostat::calculateCurrentEnergy(ParticleContainer* particles){
	Thermostat::currentEnergy = 0 ;
	particles->each([&] (Particle& p) {
		Thermostat::currentEnergy += Settings::particleTypes[p.type].mass * p.v.LengthOptimizedR3Squared() ;
	});

	Thermostat::currentEnergy = Thermostat::currentEnergy / 2;

	setCurrentTemperature(particles);

	LOG4CXX_TRACE(logger,"Actual Energy is :\t"<<Thermostat::currentEnergy <<"\t after: " << Simulator::iterations << "\t iterations");

	numberOfParticles = particles->getSize();

	//	initTargetEnergy();  //has to be initialized again if number of particles changes !!
	getStepEnergy();
	LOG4CXX_TRACE(logger, "Currently there are " << numberOfParticles << " particles.");
	if(currentEnergy != 0){
		beta = sqrt(1+(energyPerStep)/Thermostat::currentEnergy);
		LOG4CXX_TRACE(logger,"ScalingVelocity by :\t" << beta << "; current energy is " << currentEnergy << " at step " << Simulator::iterations);
	}else{
		beta = 1;
		LOG4CXX_FATAL(logger,"NO ENERGY something must have gone wrong !");
	}

}



inline void Thermostat::iterateBeta(){
	beta = sqrt(2-1/(beta*beta)); /*Ask Leo if you want to know why that works he can give you a wonderful prove*/
	LOG4CXX_TRACE(logger,"Scaling velocities by "<<beta<< "; current energy is " << currentEnergy << " at step " << Simulator::iterations);
}


void Thermostat::updateThermostate(ParticleContainer* particles) {
	if ((maxSteps == 0 )||(Simulator::iterations < maxSteps)) {
		int iterations = Simulator::iterations + 1;
		if ((iterations % stepSize) == 0) {
			calculateCurrentEnergy(particles);
		}
		//Notice: setting the velocities of the particles happens in Scenario::updateVerletPositionThermostate
		iterateBeta();
	}
}

void Thermostat::setCurrentTemperature(ParticleContainer* particles){
	double currentTemperature_arg = 2 * currentEnergy / (dimensions * numberOfParticles * BOLTZMANN);
	currentTemperature = currentTemperature_arg;
	LOG4CXX_TRACE(logger,"Temperature : " << Thermostat::currentTemperature << " degrees");
}

