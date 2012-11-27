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

log4cxx::LoggerPtr Simulator::logger = log4cxx::Logger::getLogger("Simulator");

Simulator::Simulator() {
	if(Settings::containerType == ContainerType::ParticleContainer) {
		particleContainer = new ParticleContainer();
	}
	else {
		particleContainer = new CellListContainer();
	}
	scenario = ScenarioFactory::build(Settings::scenarioType);

	scenario->setup(*particleContainer);

	LOG4CXX_TRACE(logger, "Scenario set up.");
	//pre-calculate the forces for the first update
	calculateF();

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


void Simulator::nextTimeStep() {

	// calculate new positions
	calculateX();


	//clear force accumulation vector and rearrange internal particle container structure
	particleContainer->afterPositionChanges(scenario->boundaryHandler,scenario->haloHandler);

	// calculate new forces
	calculateF();
	// calculate new velocities
	calculateV();
}
