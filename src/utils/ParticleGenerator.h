/*
 * @file ParticleGenerator.h
 *
 * @date Nov 6, 2012
 * @author Jakob Weiss
 *
 * Particle generation functions to batch create particles instead of specifying 
 */

#ifndef PARTICLEGENERATOR_H_
#define PARTICLEGENERATOR_H_

#include "Particle.h"
#include "utils/Vector.h"
#include <vector>

#include "ParticleContainer.h"

/**
 * generates particles on a regular cuboid grid
 * the velocity will be distorted by brownian motion based on brownianMean parameter
 *
 * @param container the particle container that receives the generated particles
 * @param bottomLeft the bottom left corner of the cuboid
 * @param nX1 number of particles in x1 dimension
 * @param nX2 number of particles in x2 dimension
 * @param nX3 number of particles in x3 dimension
 * @param h distance between particles (in every direction)
 * @param m mass of the particles
 * @param type type of the particles
 * @param initialVelocity base velocity of the particles
 * @param brownianMean mean velocity for brownian motion specify (0,0,0) to disable brownian motion
 */
void generateParticlesRegularCuboid(ParticleContainer& container,
		utils::Vector<double, 3> bottomLeft,
		int nX1, int nX2, int nX3,
	double h, double m, int type,
		utils::Vector<double, 3> initialVelocity,
		double brownianMean);


#endif /* PARTICLEGENERATOR_H_ */
