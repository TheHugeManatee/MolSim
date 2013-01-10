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

log4cxx::LoggerPtr FileReader::logger = log4cxx::Logger::getLogger("FileReader");

FileReader::FileReader() {
}

FileReader::~FileReader() {
}


void FileReader::readFile(ParticleContainer& container, char* filename) {
	double x[] = {0,0,0};
	double v[] = {1,1,1};
	double m = 1;
	int num_particles = 0;

	std::ifstream input_file(filename);
	string tmp_string;

	if (input_file.is_open()) {

		getline(input_file, tmp_string);
		LOG4CXX_DEBUG(logger, "Read line: " << tmp_string);

		while (tmp_string.size() == 0 || tmp_string[0] == '#') {
			LOG4CXX_DEBUG(logger,  "Skipped line: " << tmp_string);
			getline(input_file, tmp_string);
			LOG4CXX_DEBUG(logger,  "Read line: " << tmp_string);
		}

		istringstream numstream(tmp_string);
		numstream >> num_particles;
		LOG4CXX_INFO(logger, "Reading " << num_particles << " particles");
		getline(input_file, tmp_string);
		LOG4CXX_TRACE(logger, "Read line: " << tmp_string);

		for (int i = 0; i < num_particles; i++) {
			istringstream datastream(tmp_string);

			for (int j = 0; j < 3; j++) {
				datastream >> x[j];

			}
			for (int j = 0; j < 3; j++) {
				datastream >> v[j];
			}
			if (datastream.eof()) {
				LOG4CXX_FATAL(logger, "Error reading file: eof reached unexpectedly reading from line " << i << "!");
				exit(-1);
			}
			datastream >> m;
			Particle p(x, v, 0);
			container.add(p);

			getline(input_file, tmp_string);
			LOG4CXX_TRACE(logger, "Read line: " << tmp_string);
		}
		// all fixed particles read
		//now cuboid functions etc.
		while(!input_file.eof()) {
			string keyword;
			LOG4CXX_TRACE(logger, "Read line: " << tmp_string);
			istringstream lstream(tmp_string);
			lstream >> keyword;

			if(!keyword.compare("cuboid")) {
				utils::Vector<double, 3> bottomLeft, initialVelocity;
				int nX1, nX2, nX3, type;
				double h, m, bMean;

				lstream >> bottomLeft[0];
				lstream >> bottomLeft[1];
				lstream >> bottomLeft[2];
				lstream >> initialVelocity[0];
				lstream >> initialVelocity[1];
				lstream >> initialVelocity[2];
				lstream >> m;
				lstream >> type;
				lstream >> bMean;
				lstream >> nX1;
				lstream >> nX2;
				lstream >> nX3;
				lstream >> h;

				ParticleGenerator::regularCuboid(container, bottomLeft, nX1, nX2, nX3, h, type, initialVelocity, bMean);
			}
			if(!keyword.compare("particle")){
				LOG4CXX_TRACE(logger, "Generating Particle");

				Particle p(tmp_string);
				container.add(p);

			}
			getline(input_file, tmp_string);
		}
		LOG4CXX_DEBUG(logger, "Done reading file");
	} else {
		LOG4CXX_FATAL(logger, "Error: could not open file " << filename);
		exit(-1);
	}

}
