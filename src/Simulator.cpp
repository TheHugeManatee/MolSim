/*
* Simulator.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#include "Simulator.h"

#include "utils/Settings.h"

#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#ifndef NOGLVISUALIZER
	#include "outputWriter/RenderOutputWriter.h"
#endif

log4cxx::LoggerPtr Simulator::logger = log4cxx::Logger::getLogger("Simulator");

Simulator::Simulator() {
	if(Settings::containerType == ContainerType::ParticleContainer) {
		particleContainer = new ParticleContainer();
	}
	else {
		particleContainer = new CellListContainer();
	}
	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
		initTargetEnergy();
	}
	scenario = ScenarioFactory::build(Settings::scenarioType);

	scenario->setup(*particleContainer);

	particleContainer->afterPositionChanges(scenario->boundaryHandlers, scenario->haloHandler);

	LOG4CXX_TRACE(logger, "Scenario set up.");
	//pre-calculate the forces for the first update
	calculateF();
	Simulator::iterations = 0 ;
	LOG4CXX_INFO(logger, "Simulator set up.");
	LOG4CXX_INFO(logger, "Will run scenario " << Settings::scenarioType);
	LOG4CXX_INFO(logger, "World has " << particleContainer->getSize() << " particles");

	//plotParticles(0);
}

Simulator::~Simulator() {
	delete particleContainer;
	delete scenario;
}


/**
* Calculates forces for all given particles in the particle list
* and sets the net force in the object's member f
*/

void Simulator::calculateF() {
	particleContainer->eachPair(scenario->calculateForce);
	//LOG4CXX_TRACE(logger,"Finished Force Calculation" );
}


void Simulator::calculateX() {
    particleContainer->each(scenario->updatePosition);
    //LOG4CXX_TRACE(logger,"Finished Position Calculation" );
}

void Simulator::calculateV() {
	particleContainer->each(scenario->updateVelocity);
	//LOG4CXX_TRACE(logger,"Finished Velocity Calculation" );
}

void Simulator::plotParticles(int iteration) {
	outputWriter::VTKWriter vtkWriter;
	outputWriter::XYZWriter xyzWriter;


#ifndef NOGLVISUALIZER
	if(Settings::show3DVisual) {
		outputWriter::RenderOutputWriter openglView;

		openglView.plotParticles(*particleContainer, Settings::outputFilePrefix, iteration);
	}
#endif
	switch (Settings::outputFileType) {
	case OutputFileType::xyz:

		xyzWriter.plotParticles(*particleContainer, Settings::outputFilePrefix, iteration);
	break;
	case OutputFileType::vtk:

		vtkWriter.initializeOutput(particleContainer->getSize());

		particleContainer->each([&] (Particle& p) {
			vtkWriter.plotParticle(p);
		});

		vtkWriter.writeFile(Settings::outputFilePrefix, iteration);

	break;
	}

	LOG4CXX_TRACE(logger,"Plotted \t"<< iteration << "\t Particles" );
}

void Simulator::initTargetEnergy(){
	double te = Settings::thermostatSettings.targetTemperature().get();

	Simulator::targetEnergy =  3.0/2.0 * particleContainer->getSize() * te; //TODO: bolzmann konstante

}

void Simulator::calculateCurrentEnergy(){
	currentEnergy = 0;
	particleContainer->each([&] (Particle& p) {
		currentEnergy += p.m * p.v.LengthOptimizedR3Squared() ;
	});

	beta = sqrt(Simulator::targetEnergy/currentEnergy);
}

void Simulator::thermostate(){
	particleContainer->each([&] (Particle& p) {

		if(iterations % Settings::thermostatSettings.stepSize().get()){
			calculateCurrentEnergy();
		}
		p.v = p.v * beta;
	});
}



void Simulator::nextTimeStep() {

	// calculate new positions
	calculateX();

	std::function<bool (ParticleContainer &container, Particle &p)> boundaryHandlers[6];

	/*just a do nothing function TODO: find a better place for this*/
	for(int i = 0 ; i<6 ; i++){
	boundaryHandlers[i] = [] (ParticleContainer &container, Particle &p) {
		return false;
	};
	}

	//clear force accumulation vector and rearrange internal particle container structure		/*This move is ugly but necessary for periodic boundary Handling*/
	particleContainer->afterPositionChanges(scenario->boundaryHandlers, boundaryHandlers[0]);	/*Without it we wouldn't have any way to calculate forces between */
																								/*two opposite cells*/
																								/*TODO:Maybe we should seperate afterPostionChages in one method
																								*applying bounderyHandling an one to apply haloHandling an sort the cells
	// calculate new forces																		*after force calculation*/
	calculateF();

	particleContainer->afterPositionChanges(boundaryHandlers, scenario->haloHandler );


	// calculate new velocities
	calculateV();

	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
		thermostate();
	}


	iterations++;
}



