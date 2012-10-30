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
	std::list<Particle>::iterator iterator;
	iterator = particles.begin();

	while (iterator != particles.end()) {
		std::list<Particle>::iterator innerIterator = particles.begin();
		utils::Vector<double, 3> resultForce; //new force vector
		resultForce[0] = 0;
		resultForce[1] = 0;
		resultForce[2] = 0;

		while (innerIterator != particles.end()) {
			if (innerIterator != iterator) {

				Particle& p1 = *iterator;
				Particle& p2 = *innerIterator;

				utils::Vector<double, 3> xDif = p2.getX()-p1.getX();
				double normRaised3 = xDif.L2Norm()*xDif.L2Norm()*xDif.L2Norm();
				resultForce = resultForce + ((p1.getM()*p2.getM())/normRaised3) * (xDif);

			}
			++innerIterator;
		}
		(*iterator).setF(resultForce);

		++iterator;
	}
}


void Simulator::calculateX() {
	std::list<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {
		double dt = Settings::getDeltaT();
		Particle& p = *iterator;

		// insert calculation of X here!
		utils::Vector<double, 3> resultX;
		resultX= p.getX() + dt * p.getV() + dt * dt / (2 * p.getM()) * p.getF();
		p.setX(resultX);
		++iterator;
	}
}


void Simulator::calculateV() {
	std::list<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {

		Particle& p = *iterator;

		// insert calculation of velocity here!
		utils::Vector<double, 3> resultV;
		resultV = p.getV() + Settings::getDeltaT() / (2 * p.getM()) * (p.getOldF() + p.getF());
		p.setV(resultV);
		++iterator;
	}
}


void Simulator::plotParticles(int iteration) {

	std::string out_name("OutputFiles/MD_vtk");

	outputWriter::VTKWriter writer;
	writer.initializeOutput(particles.size());

	std::list<Particle>::iterator it = particles.begin();
	while(it != particles.end()) {

		writer.plotParticle(*it);

		++it;
	}

	writer.writeFile(out_name, iteration);
}

void Simulator::readInputFile(char* inputFile) {
	FileReader fileReader;
	fileReader.readFile(particles, inputFile);
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
