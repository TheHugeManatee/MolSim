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
	scenario = ScenarioFactory::build(Settings::scenarioType);

	scenario.setup(particleContainer);

	LOG4CXX_INFO(logger, "Simulator set up.");
	LOG4CXX_INFO(logger, "Will run scenario " << Settings::scenarioType);
	LOG4CXX_INFO(logger, "World has " << particleContainer.getSize() << " particles");

	//plotParticles(0);
}

Simulator::~Simulator() {

}


/**
* Calculates forces for all given particles in the particle list
* and sets the net force in the object's member f
*/

void Simulator::calculateF() {
	particleContainer.each([] (Particle& p) {
		p.old_f = p.f;
		p.f = 0;
	});

	particleContainer.eachPair(scenario.calculateForce);
	LOG4CXX_TRACE(logger,"Finished Force Calculation" );
}


void Simulator::calculateX() {
    particleContainer.each(scenario.updatePosition);
    LOG4CXX_TRACE(logger,"Finished Position Calculation" );
}

void Simulator::calculateV() {
	particleContainer.each(scenario.updateVelocity);
	LOG4CXX_TRACE(logger,"Finished Velocity Calculation" );
}

void Simulator::plotParticles(int iteration) {
	outputWriter::VTKWriter writer;

	writer.initializeOutput(particleContainer.getSize());

	particleContainer.each([&] (Particle& p) {
        writer.plotParticle(p);
    });

	writer.writeFile(Settings::outputFilePrefix, iteration);

	LOG4CXX_TRACE(logger,"Plotted \t"<< iteration << "\t Particles" );
}


void Simulator::nextTimeStep() {
	// calculate new positions
	calculateX();

	// calculate new f
	calculateF();
	// calculate new v
	calculateV();
}
