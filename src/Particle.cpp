/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"
#include <sstream>
#include <iostream>
#include "utils/Settings.h"

log4cxx::LoggerPtr Particle::logger = log4cxx::Logger::getLogger("Particle");
int Particle::createdInstances = 0;
int Particle::createdByCopy = 0;
int Particle::destroyedInstances = 0;


Particle::Particle(int type_arg) {
	type = type_arg;
	LOG4CXX_TRACE(logger, "Particle generated!");
	Particle::createdInstances++;

	memset(f_acc, 0, sizeof(f_acc));

	old_f = 0.0;
	id = -1;
}

Particle::Particle(const Particle& other) {
	x = other.x;
	x_t0 = other.x_t0;
	v = other.v;
//	f = other.f;
	old_f = other.old_f;
	type = other.type;
	LOG4CXX_TRACE(logger, "Particle generated by copy!");
	Particle::createdInstances++;
	Particle::createdByCopy++;
	id = other.id;
	memset(f_acc, 0, sizeof(f_acc));
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg
) : x(x_arg), v(v_arg), type(type_arg), old_f(0.0), id(-1), x_t0(x_arg) {

	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;

	memset(f_acc, 0, sizeof(f_acc));
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg,
		double *f_arg,
		utils::Vector<double, 3> old_f_arg,
		int id_arg
) {
	x = x_arg;
	x_t0 = x;
	v = v_arg;
	type = type_arg;

	memcpy(f_acc, f_arg, sizeof(f_acc));

	old_f = old_f_arg;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
	id = id_arg;
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg,
		int id_arg
) : x(x_arg), x_t0(x_arg), v(v_arg), type(type_arg), id(id_arg) {


	memset(f_acc, 0, sizeof(f_acc));

	old_f = 0.0;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
}

Particle::Particle(std::string importString) {

	std::istringstream lstream(importString);
	std::string t;
	lstream >> t;
	lstream >> x[0];
	lstream >> x[1];
	lstream >> x[2];
	lstream >> v[0];
	lstream >> v[1];
	lstream >> v[2];
	lstream >> f_acc[0][0];
	lstream >> f_acc[0][1];
	lstream >> f_acc[2][2];
	lstream >> old_f[0];
	lstream >> old_f[1];
	lstream >> old_f[2];
	lstream >> type;
	lstream >> id;

	x_t0 = 0;

	memset(f_acc, 0, sizeof(f_acc) - 3*sizeof(double));

	//std::cout << importString << std::endl << toStringForExport() << std::endl << std::endl;

	Particle::createdInstances++;
	LOG4CXX_TRACE(logger,"Particle generated by string");
}

Particle::~Particle() {
	LOG4CXX_TRACE(logger,"Particle destructed!");
	Particle::destroyedInstances++;
}

std::string Particle::toStringForExport() {
	std::stringstream stream;
	std::string str;
	stream << "   particle" << x << v << old_f << old_f << type <<" "<< id;
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
	stream << "Particle: X:" << x <<  " v: " << v << " old_f: " << old_f << " type: " << type;
	return stream.str();
}

bool Particle::operator ==(Particle& other) {
	if ( (x == other.x) && (v == other.v) &&
			(type == other.type) && (old_f == other.old_f)) {
		return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
	stream << p.toString();
	return stream;
}


/*0: links 1: rechts 2: vor 3: zur�ck das macht spa� das bringt gl�ck*/

bool Particle::isNeighbour(Particle &mol) {
	int otherId = mol.id;
	int breadth = Settings::particleTypes[type].membraneDescriptor.nX2 + 2;
	int length = Settings::particleTypes[type].membraneDescriptor.nX1 + 2;

	return (otherId - 1 == id) || (otherId + 1 == id) || //x1 direction
			(otherId - breadth == id) || (otherId + breadth == id) || //x2 direction
			(otherId - breadth * length == id) || (otherId + breadth * length
			== id); //x3 direction
}

/*0: leftup  1:rightdown 2:rightup 3:leftdown*/

bool Particle::isFaceDiagonal(Particle &mol) {
	int otherId = mol.id;
	int breadth = Settings::particleTypes[type].membraneDescriptor.nX2 +2;
	int length = Settings::particleTypes[type].membraneDescriptor.nX1 +2;

	return (otherId - 1 - breadth == id) || (otherId + 1 + breadth == id)
			|| (otherId - breadth + 1 == id) || (otherId + breadth - 1 == id)
			|| (otherId - breadth * length + 1 == id) || (otherId + breadth
			* length - 1 == id) || (otherId - breadth * length - 1 == id)
			|| (otherId + breadth * length + 1 == id) || (otherId - breadth
			* (length + 1) == id) || (otherId + breadth * (length - 1) == id)
			|| (otherId - breadth * (length - 1) == id) || (otherId + breadth
			* (length + 1) == id);
}


bool Particle::isSpaceDiagonal(Particle &mol) {
	int otherId = mol.id;
	int breadth = Settings::particleTypes[type].membraneDescriptor.nX2 +2;
	int length = Settings::particleTypes[type].membraneDescriptor.nX1 +2;

	return otherId - 1 - breadth - breadth * length == id || otherId - 1
			- breadth + breadth * length == id || otherId - 1 + breadth
			- breadth * length == id || otherId - 1 + breadth + breadth
			* length == id || otherId + 1 - breadth - breadth * length == id
			|| otherId + 1 - breadth + breadth * length == id || otherId + 1
			+ breadth - breadth * length == id || otherId + 1 + breadth
			+ breadth * length == id;
}


