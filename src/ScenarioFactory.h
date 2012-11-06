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

typedef struct {
	std::function<void (Particle&, Particle&)> calculateForce;
	std::function<void (Particle&)> updatePosition;
	std::function<void (Particle&)> updateVelocity;
} SimulationScenario;



class ScenarioFactory {
public:
	static SimulationScenario build(std::string type);
private:
	static std::function<void (Particle&)> verletUpdatePosition;
	static std::function<void (Particle&)> verletUpdateVelocity;
};

#endif /* SCENARIOFACTORY_H_ */
