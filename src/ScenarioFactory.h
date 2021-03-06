/*
 * @file ScenarioFactory.h
 *
 * @date Nov 6, 2012
 * @author Jakob Weiss
 */

#ifndef SCENARIOFACTORY_H_
#define SCENARIOFACTORY_H_


#include "Particle.h"
#include "FileReader.h"
#include "ParticleContainer.h"

#include "utils/SimulationConfig.h"

#include "utils/Matrix.h"

#include <log4cxx/logger.h>
#include <functional>
#include <string>
/*
enum ScenarioType {
	GRAVITY,
	LENNARD_JONES
};
*/
/**
 * this inline struct defines a scenario, which is essentially a strategy describing how the simulation logic works
 */
typedef struct {
	/**
	 * this functional calculates the force between two particles, based on some physics formula to be implemented
	 * this is usually called via a ParticleContainer 's eachPair method, looping over all unique pairs
	 * this function should add the resulting force to the force vectors of the two particles to compute the net force
	 * in each iteration
	 *
	 * @param p1 the first particle
	 * @param p2 the second particle
	 */
	std::function<void (Particle &p1, Particle&p2)> calculateForce;
	/**
	 * this method should update the position of a particle according to force and velocity in this time step
	 */
	std::function<void (Particle&)> updatePosition;
	/**
	 * this method should update the velocity of the particle according to the forces in the time step
	 */
	std::function<void (Particle&)> updateVelocity;

	/**
	 * this should perform a setup of the simulation scenario
	 * typically, this loads some input data from a file to set up the scene, but could also
	 * create some hardcoded particles or read something from the command line, etc.
	 * @param container the Simulator's ParticleContainer used in the current Simulation
	 */
	std::function<void (ParticleContainer& container)> setup;

	/**
	 * the boundary handlers for each boundary:
	 * indices are used as boundaryHandlers[axis + (inPositiveDirection?0:1)]
	 * that means [0] handles boundary at x0 = 0,
	 * [5] handles boundary at x3 = domainSize[3]
	 */
	std::function<bool (ParticleContainer &container, Particle &p)> boundaryHandlers[6];

	std::function<void (Particle &)> addAdditionalForces;

} SimulationScenario;


/**
 * @class ScenarioFactory
 *
 * This class is a factory for creating the different simulation scenarios
 * each scenario consists of a fixed set of functions handling setup and updates for each time step
 * a scenario is created using the static ScenarioFactory::build(std::string type) member function
 * this will take a string naming the required scenario, like "gravity" or "Lennard-Jones"
 *
 * @see SimulationScenario for details on scenarios in general
 *
 *
 */
class ScenarioFactory {

public:
	/**
	 * build a scenario based on a the type of scenario requested
	 * @param type the requested type of scenario
	 * 			available scenarios are:
	 * 		- gravity: simple gravity simulation, with hardcoded g = 1
	 * 		- Lennard-Jones: molecule simulation based on the Lennard-Jones potential
	 */
	static SimulationScenario *build(ScenarioType type);

	/**
	 * the static logger instance for the scenario factory
	 */
	static log4cxx::LoggerPtr logger;


	/**
	 * velocity and position update function using verlet integration
	 */
	static std::function<void (Particle&)> verletUpdatePosition;

	/**
	 * velocity position update, using thermostate influence
	 */
	static std::function<void (Particle&)> verletUpdatePositionThermostate;

	/**
	 * velocity update function using verlet integration
	 */
	static std::function<void (Particle&)> verletUpdateVelocity;

	/**
	 * force calculation based on the Lennard-Jones potential
	 */
	static std::function<void (Particle&, Particle&)> calculateLennardJonesPotentialForce;


	/**
	 * force calculation based on a smoothed Lennard-Jones potential. It basically weighs the
	 * standard Lennard Jones Potential in such a way with a sigmoid function to eliminate the
	 * sharp edge at the jump discontinuity at the cutoff point as it is already at the toe of
	 * the sigmoid function and therefore weighted with zero in this place.
	 */
	static std::function<void (Particle&, Particle&)> calculateSmoothLJ;

	/**
	 * force calculation based on the Lennard-Jones potential
	 * plus additional gravitation force along the y-axe
	 */
	static std::function<void (Particle&, Particle&)> calculateMembraneForce;
	/**
	 * force calculation using simple gravitational forces
	 */
	static std::function<void (Particle&, Particle&)> calculateGravityForce;

	/**
	 * standard setup reading particle definitions using the FileReader class
	 */
	static std::function<void (ParticleContainer &container)> basicFileReaderSetup;

	/**
	 * setup reading particle definitions using the FileReader class, along with some
	 * assertions checking the validity of the input
	 * will generate cuboids and spheres based on the Settings::generator object
	 */
	static std::function<void (ParticleContainer &container)> LennardJonesSetup;

	/**
	 * These are the handlers for a periodic boundaryHandling
	 * Particles in boundary cells have to be copied to the opposite halo cell for forceCalculations on relying cells
	 */
	static std::function<bool (ParticleContainer &container, Particle &p)> periodicHandlers[6];

	/**
	 * adds all additional forces to the force-vector of the particle. These additional forces are
	 * Force fields and gravitation.
	 */
	static std::function<void (Particle &p)> addAdditionalForces;
};


#endif /* SCENARIOFACTORY_H_ */
