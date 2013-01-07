/*
 * Molecule.h
 *
 *  Created on: 06.01.2013
 *      Author: Leo
 */

#ifndef MOLECULE_H_
#define MOLECULE_H_

#include "Particle.h"
#include "utils/Vector.h"

class Molecule: public Particle {
public:
	Molecule();
	Molecule(utils::Vector<double, 3> x_arg,
			utils::Vector<double, 3> v_arg,
			double m_arg,
			int type_arg,
			double sigma_arg,
			double epsilon_arg,
			int breadth_arg,
			int id_arg,
			int membraneId_arg,
			int stiffnessConstant_arg,
			int averageBondLength_arg);

	bool isNeighbour(Molecule mol);
	bool isDiagonal(Molecule mol);

	double stiffnessConstant;
	double averageBondLength;
	int getId();

private:
	int breadth;
};

#endif /* MOLECULE_H_ */
