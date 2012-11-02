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
#include "FileReader.h"

Simulator::Simulator() {

}

Simulator::~Simulator() {

}


/**
* Calculates forces for all given particles in the particle list
* and sets the net force in the object's member f
*/

void Simulator::calculateF() {
    ParticleContainer& pc = particleContainer;
	auto fn = [&] (Particle& p1) {
        utils::Vector<double, 3> resultForce; //new force vector
		resultForce[0] = 0;
		resultForce[1] = 0;
		resultForce[2] = 0;

        pc.each([&] (Particle& p2){
            if(p1 == p2)
                return;

            utils::Vector<double, 3> xDif = p2.getX()-p1.getX();
            double normRaised3 = xDif.L2Norm()*xDif.L2Norm()*xDif.L2Norm();
            resultForce = resultForce + ((p1.getM()*p2.getM())/normRaised3) * (xDif);
        });
        p1.setF(resultForce);
	};

	particleContainer.each(fn);
}


void Simulator::calculateX() {
    particleContainer.each([] (Particle& p) {
        double dt = Settings::deltaT;

        utils::Vector<double, 3> resultX;
		resultX= p.getX() + dt * p.getV() + dt * dt / (2 * p.getM()) * p.getF();
		p.setX(resultX);
    });
}

void Simulator::calculateV() {
	particleContainer.each([] (Particle& p) {
        utils::Vector<double, 3> resultV;
		resultV = p.getV() + Settings::deltaT / (2 * p.getM()) * (p.getOldF() + p.getF());
		p.setV(resultV);
    });
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

void Simulator::readInputFile(char* inputFile) {
	FileReader fileReader;
	fileReader.readFile(particleContainer.getContainer(), inputFile);
	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();
}


void Simulator::nextTimeStep() {
	// calculate new positions
	calculateX();

	// calculate new f
	calculateF();
	// calculate new v
	calculateV();
}
