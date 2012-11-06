/*
 * ScenarioFactory.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ScenarioFactory.h"

#include "utils/Settings.h"
#include "utils/Vector.h"

std::function<void (Particle&)> ScenarioFactory::verletUpdatePosition =
[] (Particle& p) {
	double dt = Settings::deltaT;

	utils::Vector<double, 3> resultX;
	resultX= p.getX() + dt * p.getV() + dt * dt / (2 * p.getM()) * p.getF();
	p.setX(resultX);
};

std::function<void (Particle&)> ScenarioFactory::verletUpdateVelocity = [] (Particle& p) {
    utils::Vector<double, 3> resultV;
	resultV = p.getV() + Settings::deltaT / (2 * p.getM()) * (p.getOldF() + p.getF());
	p.setV(resultV);
};

SimulationScenario ScenarioFactory::build(std::string type) {
	SimulationScenario scenario;
	if(!type.compare("gravity")) {

		//Simple gravity simulation with gravitational constant g = 1
		scenario.calculateForce = [] (Particle& p1, Particle& p2) {
	        utils::Vector<double, 3> xDif = p2.getX() - p1.getX();
	        double normRaised3 = xDif.L2Norm() * xDif.L2Norm() * xDif.L2Norm();
	        utils::Vector<double, 3> resultForce = ((p1.getM() * p2.getM()) / normRaised3) * (xDif);

	        p1.addF(resultForce);
	        resultForce = resultForce * -1;
	        p2.addF(resultForce);
		};

		scenario.setup = [&](ParticleContainer& container){
			FileReader fileReader;
			fileReader.readFile(container.getContainer(), (char*)Settings::inputFile.c_str());
			// the forces are needed to calculate x, but are not given in the input file.
			container.each(ScenarioFactory::verletUpdateVelocity);
		};

		scenario.updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario.updateVelocity = ScenarioFactory::verletUpdateVelocity;

		return scenario;
	}
	else if(!type.compare("Lennard-Jones")){
		scenario.calculateForce = [] (Particle& p1, Particle& p2) {
	        utils::Vector<double, 3> xDif = p2.getX() - p1.getX();
	        double normRaised = xDif.L2Norm();
	        utils::Vector<double, 3> resultForce;

	        p1.addF(resultForce);
	        resultForce = resultForce * -1;
	        p2.addF(resultForce);
		};

		scenario.setup = [&](ParticleContainer& container){
			FileReader fileReader;
			fileReader.readFile(container.getContainer(), (char*)Settings::inputFile.c_str());
			// the forces are needed to calculate x, but are not given in the input file.
			container.each(ScenarioFactory::verletUpdateVelocity);
		};

		scenario.updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario.updateVelocity = ScenarioFactory::verletUpdateVelocity;

		return scenario;
	}
}
