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
	f = 0.0;
	old_f = 0.0;
	id = -1;
}

Particle::Particle(const Particle& other) {
	x = other.x;
	v = other.v;
	f = other.f;
	old_f = other.old_f;
	type = other.type;
	LOG4CXX_TRACE(logger, "Particle generated by copy!");
	Particle::createdInstances++;
	Particle::createdByCopy++;
	id = other.id;
}

// Todo: maybe use initializater list instead of copy?
Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg
) : x(x_arg), v(v_arg), type(type_arg), f(0.0), old_f(0.0), id(-1) {
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;

}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg,
		utils::Vector<double, 3> f_arg,
		utils::Vector<double, 3> old_f_arg,
		int id_arg
) {
	x = x_arg;
	v = v_arg;
	type = type_arg;
	f = f_arg;
	old_f = old_f_arg;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
	id = id_arg;
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		int type_arg,
		int id_arg
) {
	x = x_arg;
	v = v_arg;
	type = type_arg;
	f = 0.0;
	old_f = 0.0;
	LOG4CXX_TRACE(logger,"Particle generated");
	Particle::createdInstances++;
	id = id_arg;
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
	lstream >> f[0];
	lstream >> f[1];
	lstream >> f[2];
	lstream >> old_f[0];
	lstream >> old_f[1];
	lstream >> old_f[2];
	lstream >> type;
	lstream >> id;

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
	stream << "Particle: X:" << x <<  " v: " << v << " f: " << f << " old_f: " << old_f << " type: " << type;
	return stream.str();
}

bool Particle::operator ==(Particle& other) {
	if ( (x == other.x) && (v == other.v) && (f == other.f) &&
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
bool Particle::isNeighbour(Particle &mol){

	int otherId = mol.id;
	int nX0 = Settings::particleTypes[type].membraneDescriptor.nX0;
	int nX1 = Settings::particleTypes[type].membraneDescriptor.nX1;
	int nX2 = Settings::particleTypes[type].membraneDescriptor.nX2;

	int x2 = id % nX2;
	int x1 = (id/nX2) % (nX1) ; //int pid = x2 + x1*nX2 + x0*nX2*nX1
	int x0 = id / (nX2*nX1);


	return  (x0 > 0       &&	(x2   +   (x1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && 	(x2   +   (x1)*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x1 > 0       &&	(x2   + (x1-1)*nX2 +   (x0)*nX2*nX1 == otherId)) ||
			(x1 < nX1 - 1 && 	(x2   + (x1+1)*nX2 +   (x0)*nX2*nX1 == otherId)) ||
			(x2 > 0       &&	(x2-1 +   (x1)*nX2 +   (x0)*nX2*nX1 == otherId)) ||
			(x2 < nX2 - 1 && 	(x2+1 +   (x1)*nX2 +   (x0)*nX2*nX1 == otherId));
}

/*0: leftup  1:rightdown 2:rightup 3:leftdown*/

bool Particle::isFaceDiagonal(Particle &mol) {
	int otherId = mol.id;
	int nX0 = Settings::particleTypes[type].membraneDescriptor.nX0;
	int nX1 = Settings::particleTypes[type].membraneDescriptor.nX1;
	int nX2 = Settings::particleTypes[type].membraneDescriptor.nX2;

	int x2 = id % nX2;
	int x1 = (id/nX2) % (nX1) ; //int pid = x2 + x1*nX2 + x0*nX2*nX1
	int x0 = id / (nX2*nX1);



	return  (x1 > 0       && x2 > 0       && (x2-1 + (x1-1)*nX2 + (x0  )*nX2*nX1 == otherId)) ||
			(x1 < nX1 - 1 && x2 > 0       && (x2-1 + (x1+1)*nX2 + (x0  )*nX2*nX1 == otherId)) ||
			(x1 > 0       && x2 < nX2 - 1 && (x2+1 + (x1-1)*nX2 + (x0  )*nX2*nX1 == otherId)) ||
			(x1 < nX1 - 1 && x2 < nX2 - 1 && (x2+1 + (x1+1)*nX2 + (x0  )*nX2*nX1 == otherId)) ||

			(x0 > 0       && x2 > 0       && (x2-1 + (x1  )*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x2 > 0       && (x2-1 + (x1  )*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x0 > 0       && x2 < nX2 - 1 && (x2+1 + (x1  )*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x2 < nX2 - 1 && (x2+1 + (x1  )*nX2 + (x0+1)*nX2*nX1 == otherId)) ||

			(x0 > 0       && x1 > 0       && (x2   + (x1-1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 > 0       && (x2   + (x1-1)*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x0 > 0       && x1 < nX1 - 1 && (x2   + (x1+1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 < nX1 - 1 && (x2   + (x1+1)*nX2 + (x0+1)*nX2*nX1 == otherId));


}

bool Particle::isSpaceDiagonal(Particle &mol) {
	int otherId = mol.id;
	int nX0 = Settings::particleTypes[type].membraneDescriptor.nX0;
	int nX1 = Settings::particleTypes[type].membraneDescriptor.nX1;
	int nX2 = Settings::particleTypes[type].membraneDescriptor.nX2;

	int x2 = id % nX2;
	int x1 = (id/nX2) % (nX1) ; //int pid = x2 + x1*nX2 + x0*nX2*nX1
	int x0 = id / (nX2*nX1);

	return  (x0 > 0       && x1 > 0       && x2 > 0       && (x2-1 + (x1-1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 > 0       && x2 > 0       && (x2-1 + (x1-1)*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x0 > 0       && x1 < nX1 - 1 && x2 > 0       && (x2-1 + (x1+1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 < nX1 - 1 && x2 > 0       && (x2-1 + (x1+1)*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x0 > 0       && x1 > 0       && x2 < nX2 - 1 && (x2+1 + (x1-1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 > 0       && x2 < nX2 - 1 && (x2+1 + (x1-1)*nX2 + (x0+1)*nX2*nX1 == otherId)) ||
			(x0 > 0       && x1 < nX1 - 1 && x2 < nX2 - 1 && (x2+1 + (x1+1)*nX2 + (x0-1)*nX2*nX1 == otherId)) ||
			(x0 < nX0 - 1 && x1 < nX1 - 1 && x2 < nX2 - 1 && (x2+1 + (x1+1)*nX2 + (x0+1)*nX2*nX1 == otherId));
}

