/*
 * Settings.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "Settings.h"

//variable definitions and  default settings
double Settings::deltaT = 0.0014;
double Settings::startTime = 0;
double Settings::endTime = 100.0;
int Settings::snapshotSkips = 10;
bool Settings::disableOutput = false;
double Settings::sigma = 1;
double Settings::epsilon = 5;
std::string Settings::scenarioType = "gravity";
std::string Settings::configFile = "config.cfg";
std::string Settings::inputFile = "eingabe-sonne.txt";
std::string Settings::testCase = "";
std::string Settings::loggerConfigFile = "";
std::string Settings::outputFilePrefix = "OutputFiles/MD_vtk_";

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * static class containing all global settings relevant to simulation and
 * material etc.
 * parameters can be configured through the command line and through a config
 * standard config filename is config.cfg
 */
void Settings::initSettings(int argc, char* argv[]) {

	//Look if a config file parameter was specified
	for(int i=0; i < argc; i++) {
		if(strcmp(argv[i], "configFile") == 0 && argc > i + 1)
			Settings::configFile = argv[i+1];
	}

	std::ifstream cfgFile;
	cfgFile.open(Settings::configFile, std::ifstream::in);
	if(cfgFile.is_open()) {
		std::string var;
		while(!cfgFile.eof())
		{
			char comment[256];
			cfgFile >> var;

			//ignore everything in the line after #
			if(!var.compare("#")) cfgFile.getline(comment, sizeof(comment));

			if(!var.compare("deltaT"))	cfgFile >> Settings::deltaT;
			if(!var.compare("endTime"))	cfgFile >> Settings::endTime;
			if(!var.compare("scenarioType")) cfgFile >> Settings::scenarioType;
			if(!var.compare("disableOutput")) cfgFile >> Settings::disableOutput;
			if(!var.compare("inputFile")) cfgFile >> Settings::inputFile;
			if(!var.compare("epsilon")) cfgFile >> Settings::epsilon;
			if(!var.compare("sigma")) cfgFile >> Settings::sigma;
			if(!var.compare("snapshotSkips")) cfgFile >> Settings::snapshotSkips;
			if(!var.compare("loggerConfigFile")) cfgFile >> Settings::loggerConfigFile;
			if(!var.compare("outputFilePrefix")) cfgFile >> Settings::outputFilePrefix;

		}

		cfgFile.close();

		//override parameters from the config file and default values
		//with values from the command line
		for(int i=0; i < argc; i++) {
			if(strcmp(argv[i], "-deltaT") == 0 && argc > i + 1)
				Settings::deltaT = atof(argv[i+1]);
			if(strcmp(argv[i], "-endTime") == 0 && argc > i + 1)
				Settings::endTime = atof(argv[i+1]);
			if(strcmp(argv[i], "-scenarioType") == 0 && argc > i + 1)
				Settings::scenarioType = argv[i+1];
			if(strcmp(argv[i], "-inputFile") == 0 && argc > i + 1)
				Settings::inputFile = argv[i+1];
			if(strcmp(argv[i], "-disableOutput") == 0 && argc > i + 1)
				Settings::disableOutput = atoi(argv[i+1]);
			if(strcmp(argv[i], "-test") == 0 && argc > i + 1)
				Settings::testCase = argv[i+1];
			if(strcmp(argv[i], "-outputFilePrefix") == 0 && argc > i + 1)
				Settings::outputFilePrefix = argv[i+1];
		}

	}
	else
		std::cout << "Cannot open " << Settings::configFile << std::endl;

	assert(Settings::deltaT != 0);//timestep needs to be non-zero

	assert(sgn(Settings::deltaT) == sgn(Settings::endTime));	//deltaT and endTime have the same sign, otherwise the
}
