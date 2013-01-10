/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "utils/Vector.h"
#include <log4cxx/logger.h>

#include <string>

class Particle {

private:
	static log4cxx::LoggerPtr logger;

public:
	/** the position of the particle */
	utils::Vector<double, 3> x;

	/** the velocity of the particle */
	utils::Vector<double, 3> v;

	/** the force effective on this particle */
	utils::Vector<double, 3> f;

	/** the force which was effective on this particle */
	utils::Vector<double, 3> old_f;

	/** type of the particle. Use it for whatever you want (e.g. to seperate
	 * molecules belonging to different bodies, matters, and so on)
	 */
	int type;

	/**
	 * some id of the particle
	 */
	int id;

	/**
	 * counter variable for created instances
	 */
	static int createdInstances;
	/**
	 * counter variable for called copy constructors
	 */
	static int createdByCopy;
	/**
	 * counter variable for destroyed instances
	 */
	static int destroyedInstances;

	Particle(int type = 0);

	Particle(const Particle& other);

	Particle(const std::string importString);

	Particle(
			//to create a complete instance of a particle with all arguments set (including force and so on)
			utils::Vector<double, 3> x_arg,
			utils::Vector<double, 3> v_arg,
			int type_arg,
			utils::Vector<double, 3> f_arg,
			utils::Vector<double, 3> old_f_arg,
			int id_arg  = -1
	);


	Particle(
			// for visualization, we need always 3 coordinates
			// -> in case of 2d, we use only the first and the second
			utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        int type
	);

	Particle(
			// for visualization, we need always 3 coordinates
			// -> in case of 2d, we use only the first and the second
			utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        int type_arg,
	        int id_arg
	);

	~Particle();

	bool operator==(Particle& other);

	std::string toStringForExport();
	std::string toString();


	bool isNeighbour(Particle &mol);
	bool isFaceDiagonal(Particle &mol);
	bool isSpaceDiagonal(Particle &mol);
};

std::ostream& operator<<(std::ostream& stream, Particle& p);

#endif /* PARTICLE_H_ */
