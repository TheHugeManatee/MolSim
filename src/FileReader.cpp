/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"
#include "utils/Vector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "utils/ParticleGenerator.h"

using namespace std;

FileReader::FileReader() {
}

FileReader::~FileReader() {
}


void FileReader::readFile(std::vector<Particle>& particles, char* filename) {
	double x[] = {0,0,0};
	double v[] = {1,1,1};
	double m = 1;
    int num_particles = 0;

    std::ifstream input_file(filename);
    string tmp_string;

    if (input_file.is_open()) {

    	getline(input_file, tmp_string);
    	cout << "Read line: " << tmp_string << endl;

    	while (tmp_string.size() == 0 || tmp_string[0] == '#') {
    		getline(input_file, tmp_string);
    		cout << "Read line: " << tmp_string << endl;
    	}

    	istringstream numstream(tmp_string);
    	numstream >> num_particles;
    	cout << "Reading " << num_particles << "." << endl;
    	getline(input_file, tmp_string);
    	cout << "Read line: " << tmp_string << endl;

    	for (int i = 0; i < num_particles; i++) {
    		istringstream datastream(tmp_string);

    		for (int j = 0; j < 3; j++) {
    			datastream >> x[j];

    		}
    		for (int j = 0; j < 3; j++) {
    			datastream >> v[j];
    		}
    		if (datastream.eof()) {
    			cout << "Error reading file: eof reached unexpectedly reading from line " << i << endl;
    			exit(-1);
    		}
    		datastream >> m;
    		Particle p(x, v, m);
    		particles.push_back(p);

    		getline(input_file, tmp_string);
    		cout << "Read line: " << tmp_string << endl;
    	}
    	//read all fixed particles
    	//now cuboid functionals etc.
		string keyword;
    	while(!input_file.eof()) {
    		istringstream lstream(tmp_string);
    		lstream >> keyword;
    		if(!keyword.compare("cuboid")) {
    			utils::Vector<double, 3> bottomLeft, initialVelocity;
    			int nX1, nX2, nX3;
    			double h, m, bMean;

    			lstream >> bottomLeft[0];
    			lstream >> bottomLeft[1];
    			lstream >> bottomLeft[2];
    			lstream >> initialVelocity[0];
    			lstream >> initialVelocity[1];
    			lstream >> initialVelocity[2];
    			lstream >> m;
    			lstream >> bMean;
    			lstream >> nX1;
    			lstream >> nX2;
    			lstream >> nX3;
    			lstream >> h;

    			cout << bottomLeft << " " << initialVelocity << " " << bMean << " " << nX1 << " " << nX2 << " " << nX3 << " " << h << endl;

    			cout << "Generating " << nX1*nX2*nX3 << " particles on a regular cuboid" << endl;

    			generateParticlesRegularCuboid(particles, bottomLeft, nX1, nX2, nX3, h, m, initialVelocity, bMean);
        		getline(input_file, tmp_string);
    		}
    	}
    } else {
    	std::cout << "Error: could not open file " << filename << std::endl;
    	exit(-1);
    }

}
