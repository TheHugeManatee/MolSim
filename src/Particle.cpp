/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"
#include <sstream>
#include <iostream>

log4cxx::LoggerPtr Particle::logger = log4cxx::Logger::getLogger("Particle");
int Particle::createdInstances = 0;
int Particle::createdByCopy = 0;
int Particle::destroyedInstances = 0;


Particle::Particle(int type_arg) {
	type = type_arg;
	LOG4CXX_TRACE(logger, "Particle generated!");
	Particle::createdInstances++;
	f = 0.0;
	old_f = 0.0;
	m = 1.0;
	sigma = 1;
	epsilon = 1;
	membraneId = -1;
	id = -1;
}

Particle::Particle(const Particle& other) {
	x = other.x;
	v = other.v;
	f = other.f;
	old_f = other.old_f;
	m = other.m;
	type = other.type;
	sigma = other.sigma;
	epsilon = other.epsilon;
	LOG4CXX_TRACE(logger, "Particle generated by copy!");
	Particle::createdInstances++;
	Particle::createdByCopy++;
	membraneId = other.membraneId;
	id = other.id;
}

// Todo: maybe use initializater list instead of copy?
Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		double m_arg,
		int type_arg
) {
	x = x_arg;
	v = v_arg;
	m = m_arg;
	type = type_arg;
	f = 0.0;
	old_f = 0.0;
	sigma = 1;
	epsilon = 1;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
	membraneId = -1;
	id = -1;
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		double m_arg,
		int type_arg,
		double sigma_arg,
		double epsilon_arg
) {
	x = x_arg;
	v = v_arg;
	m = m_arg;
	type = type_arg;
	sigma = sigma_arg;
	epsilon = epsilon_arg;
	f = 0.0;
	old_f = 0.0;
	membraneId = -1;
	id = -1;

	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		double m_arg,
		int type_arg,
		utils::Vector<double, 3> f_arg,
		utils::Vector<double, 3> old_f_arg,
		double sigma_arg,
		double epsilon_arg
) {
	x = x_arg;
	v = v_arg;
	m = m_arg;
	type = type_arg;
	sigma = sigma_arg;
	epsilon = epsilon_arg;
	f = f_arg;
	old_f = old_f_arg;
	membraneId = -1;
	id = -1;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
}


Particle::~Particle() {
	LOG4CXX_TRACE(logger,"Particle destructed!");
	Particle::destroyedInstances++;
}




std::string Particle::toStringForExport() {
	std::stringstream stream;
	std::string str;
	stream << "   particle" << x << v << f << old_f << type<< " " << sigma<< " " << epsilon << " " << m;
	str = stream.str();
	for (size_t i=0; i < str.length(); i++){
		if(str.at(i) == '[' ||str.at(i) == ']' ||str.at(i) == ';'){
			str.at(i) = ' ';
		}
	}
	return str;
}

std::string Particle::toString() {
	std::stringstream stream;
	stream << "Particle: X:" << x <<  " v: " << v << " f: " << f << " old_f: " << old_f << " type: " << type;
	return stream.str();
}

bool Particle::operator ==(Particle& other) {
	if ( (x == other.x) && (v == other.v) && (f == other.f) &&
			(type == other.type) && (m == other.m) && (old_f == other.old_f)) {
		return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
	stream << p.toString();
	return stream;
}
