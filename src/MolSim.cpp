
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"

#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);


double start_time = 0;
double end_time = 100.000;
double delta_t = 0.014;

std::list<Particle> particles;


int main(int argc, char* argsv[]) {

	cout << "Hello from MolSim for PSE!" << endl;
	if (argc != 2) {
		cout << "Errounous programme call! " << endl;
		cout << "./molsym filename" << endl;
	}

	FileReader fileReader;
	fileReader.readFile(particles, argsv[1]);
	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();

	double current_time = start_time;

	int iteration = 0;

	 // for this loop, we assume: current x, current f and current v are known
	while (current_time < end_time) {
		// calculate new x
		calculateX();

		// calculate new f
		calculateF();
		// calculate new v
		calculateV();

		iteration++;
		if (iteration % 10 == 0) {
			plotParticles(iteration);
		}
		cout << "Iteration " << iteration << " finished." << endl;

		current_time += delta_t;
	}

	cout << "output written. Terminating..." << endl;
	return 0;
}

/**
* Calculates forces for all given particles in the particle list
* and sets the net force in the object's member f
*/

void calculateF() {
	list<Particle>::iterator iterator;
	iterator = particles.begin();

	while (iterator != particles.end()) {
		list<Particle>::iterator innerIterator = particles.begin();
		utils::Vector<double, 3> resultForce; //new force vector
		resultForce[0] = 0;
		resultForce[1] = 0;
		resultForce[2] = 0;

		while (innerIterator != particles.end()) {
			if (innerIterator != iterator) {

				Particle& p1 = *iterator;
				Particle& p2 = *innerIterator;

				utils::Vector<double, 3> xDif = p1.getX()-p2.getX();
				double normRaised3 = xDif.L2Norm()*xDif.L2Norm()*xDif.L2Norm();
				resultForce = resultForce + ((p1.getM()*p2.getM())/normRaised3) * xDif;

			}
			++innerIterator;
		}
		(*iterator).setF(resultForce);

		++iterator;
	}
}


void calculateX() {
	list<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {

		Particle& p = *iterator;

		// insert calculation of X here!
		utils::Vector<double, 3> resultX;
		resultX= p.getX() + delta_t * p.getV() + delta_t * delta_t / (2 * p.getM()) * p.getF();
		p.setX(resultX);
		++iterator;
	}
}


void calculateV() {
	list<Particle>::iterator iterator = particles.begin();
	while (iterator != particles.end()) {

		Particle& p = *iterator;

		// insert calculation of velocity here!
		utils::Vector<double, 3> resultV;
		resultV = p.getV() + delta_t / (2 * p.getM()) * (p.getOldF() + p.getF());
		p.setV(resultV);
		++iterator;
	}
}


void plotParticles(int iteration) {

	string out_name("OutputFiles/MD_vtk");

	outputWriter::VTKWriter writer;
	writer.initializeOutput(particles.size());

	list<Particle>::iterator it = particles.begin();
	while(it != particles.end()) {

		writer.plotParticle(*it);

		++it;
	}

	writer.writeFile(out_name, iteration);
}
