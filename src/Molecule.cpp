/*
 * Molecule.cpp
 *
 *  Created on: 06.01.2013
 *      Author: Leo
 */

#include "Molecule.h"

#define DUMMYMOLECULE -1


Molecule::Molecule(utils::Vector<double, 3> x_arg,
		utils::Vector<double, 3> v_arg,
		double m_arg,
		int type_arg,
		double sigma_arg,
		double epsilon_arg,
		int breadth_arg,
		int length_arg,
		int id_arg,
		int membraneId_arg,
		int stiffnessConstant_arg,
		int averageBondLength_arg) {
	x = x_arg;
	v = v_arg;
	m = m_arg;
	type = type_arg;
	sigma = sigma_arg;
	epsilon = epsilon_arg;
	membraneId = membraneId_arg;
	breadth=breadth_arg;
	length=length_arg;
	id=id_arg;
	std::cout << "ID :" << id << std::endl;
	stiffnessConstant = stiffnessConstant_arg;
	averageBondLength = averageBondLength_arg;
}


/*0: links 1: rechts 2: vor 3: zurŸck das macht spa§ das bringt glŸck*/
bool Molecule::isNeighbour(Molecule &mol){
	int otherId = mol.id;
	return (otherId-1==id) || (otherId+1==id) || //x1 direction
			(otherId - breadth == id) || (otherId + breadth == id) || //x2 direction
			(otherId - breadth*length == id) || (otherId + breadth*length == id); //x3 direction
}

/*0: leftup  1:rightdown 2:rightup 3:leftdown*/

bool Molecule::isFaceDiagonal(Molecule &mol) {
	int otherId = mol.id;
	return (otherId - 1 - breadth == id) || (otherId + 1 + breadth == id)
			|| (otherId - breadth + 1 == id) || (otherId + breadth - 1 == id)

			|| (otherId - breadth*length + 1 == id) || (otherId + breadth*length - 1 == id)
			|| (otherId - breadth*length - 1 == id) || (otherId + breadth*length + 1 == id)

			|| (otherId - breadth*(length + 1) == id) || (otherId + breadth*(length - 1) == id)
			|| (otherId - breadth*(length - 1) == id) || (otherId + breadth*(length + 1) == id);
}

bool Molecule::isSpaceDiagonal(Molecule &mol) {
	int otherId = mol.id;
	return 	otherId - 1 - breadth - breadth*length == id ||
			otherId - 1 - breadth + breadth*length == id ||
			otherId - 1 + breadth - breadth*length == id ||
			otherId - 1 + breadth + breadth*length == id ||
			otherId + 1 - breadth - breadth*length == id ||
			otherId + 1 - breadth + breadth*length == id ||
			otherId + 1 + breadth - breadth*length == id ||
			otherId + 1 + breadth + breadth*length == id;
}

int Molecule::getId(){
	return id;
}
