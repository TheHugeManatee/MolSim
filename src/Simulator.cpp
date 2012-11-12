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

Simulator::Simulator() {
	scenario = ScenarioFactory::build(Settings::scenarioType);

	scenario.setup(particleContainer);

	plotParticles(0);
}

Simulator::~Simulator() {

}


/**
* Calculates forces for all given particles in the particle list
* and sets the net force in the object's member f
*/

void Simulator::calculateF() {

	particleContainer.each([] (Particle& p) {
		auto f = p.getF();
		f[0] = 0;
		f[1] = 0;
		f[2] = 0;
	});

	particleContainer.eachPair(scenario.calculateForce);

}


void Simulator::calculateX() {
    particleContainer.each(scenario.updatePosition);
}

void Simulator::calculateV() {
	particleContainer.each(scenario.updateVelocity);
}

void Simulator::plotParticles(int iteration) {

	std::string out_name("OutputFiles/MD_vtk");

	outputWriter::VTKWriter writer;

	writer.initializeOutput(particleContainer.getContainer().size());

	particleContainer.each([&] (Particle& p) {
        writer.plotParticle(p);
    });

	writer.writeFile(out_name, iteration);
}


void Simulator::nextTimeStep() {
	// calculate new positions
	calculateX();

	// calculate new f
	calculateF();
	// calculate new v
	calculateV();
}
