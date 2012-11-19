/*
 * ScenarioFactory.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#include "ScenarioFactory.h"

#include "utils/ParticleGenerator.h"

#include "utils/Settings.h"
#include "utils/Vector.h"
#include <cstdlib>
#include <cassert>


log4cxx::LoggerPtr ScenarioFactory::logger = log4cxx::Logger::getLogger("ScenarioFactory");

std::function<void (Particle&)> ScenarioFactory::verletUpdatePosition =
[] (Particle& p) {
	double dt = Settings::deltaT;

	utils::Vector<double, 3> resultX;
	resultX= p.x + dt * p.v + dt * dt / (2 * p.m) * p.f;
	p.x = resultX;
};

std::function<void (Particle&)> ScenarioFactory::verletUpdateVelocity = [] (Particle& p) {
    utils::Vector<double, 3> resultV;
	resultV = p.v + Settings::deltaT / (2 * p.m) * (p.old_f + p.f);
	p.v = resultV;
};

std::function<void (Particle&, Particle&)> ScenarioFactory::calculateLennardJonesPotentialForce = [] (Particle& p1, Particle& p2) {
    utils::Vector<double, 3> xDif = p2.x - p1.x;
    double norm = xDif.L2Norm();
    double sigmaNormalized = Settings::sigma/norm;
    double sigmaNormailzedRaisedBySix = sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized*sigmaNormalized;
    utils::Vector<double, 3> resultForce = (24*Settings::epsilon / (norm * norm)) * ((sigmaNormailzedRaisedBySix) - 2 * (sigmaNormailzedRaisedBySix * sigmaNormailzedRaisedBySix))*xDif;
    p1.f = p1.f + resultForce;

    //resultForce = resultForce * -1;
    p2.f = p2.f - resultForce;
};

std::function<void (Particle&, Particle&)> ScenarioFactory::calculateGravityForce = [] (Particle& p1, Particle& p2) {
	utils::Vector<double, 3> xDif = p2.x - p1.x;
	double normRaised3 = xDif.L2Norm() * xDif.L2Norm() * xDif.L2Norm();
	utils::Vector<double, 3> resultForce = ((p1.m * p2.m) / normRaised3) * (xDif);

	p1.f = p1.f + resultForce;

	p2.f = p2.f - resultForce;
};

std::function<void (ParticleContainer &container)> ScenarioFactory::basicFileReaderSetup = [](ParticleContainer& container){
	LOG4CXX_DEBUG(logger, "Performing basic scenario setup");
	FileReader fileReader;
	fileReader.readFile(container, (char*)Settings::inputFile.c_str());
	// the forces are needed to calculate x, but are not given in the input file.
	container.each(ScenarioFactory::verletUpdateVelocity);
};


std::function<void (ParticleContainer &container)> ScenarioFactory::LennardJonesSetup = [](ParticleContainer& container){
	LOG4CXX_DEBUG(logger, "Performing Lennard-Jones setup");
	FileReader fileReader;
	fileReader.readFile(container, (char*)Settings::inputFile.c_str());
	// the forces are needed to calculate x, but are not given in the input file.
	container.each(ScenarioFactory::verletUpdateVelocity);

	LOG4CXX_TRACE(logger, "Cuboid generation:");
	for(auto it = Settings::generator.cuboid().begin();
			it != Settings::generator.cuboid().end();
			++it) {

		auto c = (*it);
		double bl[] = {c.initialVelocity().x1(), c.initialVelocity().x2(), c.initialVelocity().x3()};
		double v[] = {c.bottomLeft().x1(), c.bottomLeft().x2(), c.bottomLeft().x3()};
		ParticleGenerator::regularCuboid(container,
				utils::Vector<double, 3> (bl),
				c.nX().x1(), c.nX().x2(), c.nX().x3(),
				c.stepWidth(), c.mass(), c.type(),
				utils::Vector<double, 3> (v),
				c.brownianMeanVelocity()
		);
	}
	LOG4CXX_TRACE(logger, "Generation finished!");

	//TODO: sphere generation

	assert(Settings::epsilon > 0);
	assert(Settings::sigma > 0);

};

SimulationScenario ScenarioFactory::build(std::string type) {
	SimulationScenario scenario;
	if(!type.compare("gravity")) {
		//Simple gravity simulation with gravitational constant g = 1
		scenario.calculateForce = ScenarioFactory::calculateGravityForce;

		scenario.setup = ScenarioFactory::basicFileReaderSetup;

		scenario.updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario.updateVelocity = ScenarioFactory::verletUpdateVelocity;
		LOG4CXX_INFO(logger,"Built Gravity Scenario");
	}
	else if(!type.compare("Lennard-Jones")){
		scenario.calculateForce = ScenarioFactory::calculateLennardJonesPotentialForce;

		scenario.setup = ScenarioFactory::LennardJonesSetup;

		scenario.updatePosition = ScenarioFactory::verletUpdatePosition;
		scenario.updateVelocity = ScenarioFactory::verletUpdateVelocity;

		LOG4CXX_INFO(logger,"Built Lennard-Jones Scenario");
	} else {
		LOG4CXX_FATAL(logger, "Unknown Simulation type!");
		exit(-1);
	}
	return scenario;
}
