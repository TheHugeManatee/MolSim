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

	/** the mass of this particle */
	double m;

	/** type of the particle. Use it for whatever you want (e.g. to seperate
	 * molecules belonging to different bodies, matters, and so on)
	 */
	int type;

	/**
	 * type specific lennard jones parameter
	 */
	double sigma;
	double epsilon;

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

	Particle(
			//to create a complete instance of a particle with all arguments set (including force and so on)
			utils::Vector<double, 3> x_arg,
			utils::Vector<double, 3> v_arg,
			double m_arg,
			int type_arg,
			utils::Vector<double, 3> f_arg,
			utils::Vector<double, 3> old_f_arg,
			double sigma_arg,
			double epsilon_arg
	);

	Particle(
			// for visualization, we need always 3 coordinates
			// -> in case of 2d, we use only the first and the second
			utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        double m_arg,
	        int type = 0
	);

	Particle(
			// for visualization, we need always 3 coordinates
			// -> in case of 2d, we use only the first and the second
			utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        double m_arg,
	        int type,
	        double sigma,
	        double epsilon
	);

	virtual ~Particle();

	bool operator==(Particle& other);

	std::string toStringForExport();
	std::string toString();
};

std::ostream& operator<<(std::ostream& stream, Particle& p);

#endif /* PARTICLE_H_ */
