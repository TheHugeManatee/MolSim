/*
 * ScenarioFactory.h
 *
 *  Created on: Nov 6, 2012
 *      Author: j
 */

#ifndef SCENARIOFACTORY_H_
#define SCENARIOFACTORY_H_

#include <functional>
#include <string>
#include "Particle.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include <log4cxx/logger.h>


typedef struct {
	std::function<void (Particle&, Particle&)> calculateForce;
	std::function<void (Particle&)> updatePosition;
	std::function<void (Particle&)> updateVelocity;

	std::function<void (ParticleContainer&)> setup;
} SimulationScenario;



class ScenarioFactory {
public:
	static SimulationScenario build(std::string type);
	static log4cxx::LoggerPtr logger;

//private:
	static std::function<void (Particle&)> verletUpdatePosition;
	static std::function<void (Particle&)> verletUpdateVelocity;

	static std::function<void (Particle&, Particle&)> calculateLennardJonesPotentialForce;
	static std::function<void (Particle&, Particle&)> calculateGravityForce;

	static std::function<void (ParticleContainer &container)> basicFileReaderSetup;
	static std::function<void (ParticleContainer &container)> LennardJonesSetup;
};


#endif /* SCENARIOFACTORY_H_ */
