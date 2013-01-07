/*
 * Molecule.cpp
 *
 *  Created on: 06.01.2013
 *      Author: Leo
 */

#include "Molecule.h"

#define DUMMYMOLECULE -1

Molecule::Molecule() {
}

Molecule::Molecule(utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		double m_arg,
		int type_arg,
		double sigma_arg,
		double epsilon_arg,
		int breadth_arg,
		int id_arg,
		int membraneId_arg,
		int stiffnessConstant_arg,
		int averageBondLength_arg){
	x = x_arg;
	v = v_arg;
	m = m_arg;
	type = type_arg;
	sigma = sigma_arg;
	epsilon = epsilon_arg;
	membraneId = membraneId_arg;
	breadth=breadth_arg;
	id=id_arg;
	std::cout << "ID :" << id << std::endl;
	stiffnessConstant=stiffnessConstant_arg;
	averageBondLength=averageBondLength_arg;
}


/*0: links 1: rechts 2: vor 3: zurŸck das macht spa§ das bringt glŸck*/
bool Molecule::isNeighbour(Molecule mol){
	int otherId = mol.id;
	if((otherId-1==id) || (otherId+1==id) || (otherId-breadth==id) || (otherId+breadth==id)){
		return true;
	}
	return false;
}

/*0: leftup  1:rightdown 2:rightup 3:leftdown*/

bool Molecule::isDiagonal(Molecule mol){
	int otherId = mol.id;
	if((otherId-1-breadth==id) || (otherId+1+breadth==id) || (otherId-breadth+1==id) || (otherId+breadth-1==id)){
		return true;
	}
	return false;
}

int Molecule::getId(){
	return id;
}
