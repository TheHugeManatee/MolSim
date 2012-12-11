/*
 * Settings.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#include "Settings.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>

//Forward declarations
extern void initializeLogger();//This is implemented in MolSim.cpp


//variable definitions and  default settings
double Settings::deltaT = 0.0014;
double Settings::startTime = 0;
double Settings::endTime = 100.0;
int Settings::outputFrequency = 10;
bool Settings::disableOutput = false;
double Settings::sigma = 1;
double Settings::epsilon = 5;
ScenarioType Settings::scenarioType = ScenarioType::Gravity;
std::string Settings::configFile = "simulationConfig.xml";
std::string Settings::inputFile = "eingabe-sonne.txt";
std::string Settings::testCase = "";
std::string Settings::loggerConfigFile = "";
std::string Settings::outputFilePrefix = "OutputFiles/MD_vtk_";
OutputFileType Settings::outputFileType = OutputFileType::vtk;
double Settings::rCutoff = 3;
utils::Vector<double, 3> Settings::domainSize = 50;
ContainerType Settings::containerType = ContainerType::ParticleContainer;
BoundaryConditionType Settings::boundaryCondition[6] = {
		BoundaryConditionType::Outflow,
		BoundaryConditionType::Outflow,
		BoundaryConditionType::Outflow,
		BoundaryConditionType::Outflow,
		BoundaryConditionType::Outflow,
		BoundaryConditionType::Outflow
};
bool Settings::show3DVisual = false;
bool Settings::encodeCellsInType = false;

SimulationConfig::GeneratorType Settings::generator;

log4cxx::LoggerPtr Settings::logger = log4cxx::Logger::getLogger("Settings");

/**
 * the signum function
 * returns -1, 0 or 1
 * branchless and typesafe
 */
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * static class containing all global settings relevant to simulation and
 * material etc.
 * parameters can be configured through the command line and through a config
 * standard config filename is config.cfg
 */
#include <exception>
void Settings::initSettings(int argc, char* argv[]) {

	//Look if a config file parameter was specified
	for(int i=0; i < argc; i++) {
		if(strcmp(argv[i], "-configFile") == 0 && argc > i + 1)
			Settings::configFile = argv[i+1];
	}

	if(!Settings::configFile.substr(Settings::configFile.length() - 4, 4).compare(".xml")) {
		parseXmlFile(Settings::configFile);
	}
	else {
		parseCfgFile(Settings::configFile);
	}


	//override parameters from the config file and default values
	//with values from the command line
	for(int i=0; i < argc; i++) {
		if(strcmp(argv[i], "-deltaT") == 0 && argc > i + 1)
			Settings::deltaT = atof(argv[i+1]);
		if(strcmp(argv[i], "-endTime") == 0 && argc > i + 1)
			Settings::endTime = atof(argv[i+1]);
		if(strcmp(argv[i], "-inputFile") == 0 && argc > i + 1)
			Settings::inputFile = argv[i+1];
		if(strcmp(argv[i], "-disableOutput") == 0)
			Settings::disableOutput = true;
		if(strcmp(argv[i], "-test") == 0 && argc > i + 1)
			Settings::testCase = argv[i+1];
		if(strcmp(argv[i], "-outputFilePrefix") == 0 && argc > i + 1)
			Settings::outputFilePrefix = argv[i+1];
		if(strcmp(argv[i], "-visualize") == 0)
			Settings::show3DVisual = true;
		if(strcmp(argv[i], "-colorByCell") == 0)
			Settings::encodeCellsInType = true;
	}

	//Re-initialize the logger with possibly new configuration file
	initializeLogger();

	LOG4CXX_DEBUG(logger, "Configuration loaded: " << Settings::toString());

	assert(Settings::deltaT != 0);//timestep needs to be non-zero

	assert(sgn(Settings::deltaT) == sgn(Settings::endTime));	//deltaT and endTime have the same sign, otherwise the
}


void Settings::parseXmlFile(std::string cfgFile) {
	LOG4CXX_DEBUG(logger, "Loading Configuration from .xml file " << cfgFile);
	 try {
	    std::auto_ptr<SimulationConfig> xmlCfg (simulationConfig(cfgFile, xml_schema::Flags::dont_validate));

	    Settings::deltaT = xmlCfg->deltaT();
	    Settings::endTime = xmlCfg->endTime();
	    Settings::outputFrequency = xmlCfg->outputFrequency();
	    Settings::disableOutput = xmlCfg->disableOutput();
	    Settings::loggerConfigFile = xmlCfg->loggerConfigFile();
	    Settings::outputFilePrefix = xmlCfg->outputFilePrefix();
	    Settings::scenarioType = xmlCfg->scenarioType();
	    Settings::inputFile = xmlCfg->inputFile();
	    Settings::domainSize[0] = xmlCfg->domainSize().x0();
	    Settings::domainSize[1] = xmlCfg->domainSize().x1();
	    Settings::domainSize[2] = xmlCfg->domainSize().x2();
	    Settings::boundaryCondition[0] = xmlCfg->boundaryHandling().right();
	    Settings::boundaryCondition[1] = xmlCfg->boundaryHandling().left();
	    Settings::boundaryCondition[2] = xmlCfg->boundaryHandling().top();
	    Settings::boundaryCondition[3] = xmlCfg->boundaryHandling().bottom();
	    Settings::boundaryCondition[4] = xmlCfg->boundaryHandling().back();
	    Settings::boundaryCondition[5] = xmlCfg->boundaryHandling().front();
	    Settings::containerType = xmlCfg->containerType();
	    Settings::outputFileType = xmlCfg->outputFileType();
	    Settings::rCutoff = xmlCfg->cutoffRadius();

	    Settings::generator = xmlCfg->generator();
	    Settings::thermostat = xmlCfg->thermostat();
	    Settings::epsilon = xmlCfg->epsilon();
	    Settings::sigma = xmlCfg->sigma();
	  }
	  catch (const xml_schema::Exception &e)
	  {
	    LOG4CXX_ERROR(logger, "Error when parsing xml input file: " << e);
	    return;
	  }
}

void Settings::parseCfgFile(std::string cfgFile) {
	LOG4CXX_DEBUG(logger, "Loading Configuration from .cfg file " << cfgFile);
	LOG4CXX_WARN(logger, "Loading from a .cfg file. Note that the preferred way to load configurations is through a .xml file." <<
			"New features will likely be impelemented in the xml reader only!");

	std::ifstream cfgFileStream;
	cfgFileStream.open(cfgFile, std::ifstream::in);
	if(cfgFileStream.is_open()) {
		std::string var;
		while(!cfgFileStream.eof())
		{
			char comment[256];
			cfgFileStream >> var;

			//ignore everything in the line after #
			if(!var.compare("#")) cfgFileStream.getline(comment, sizeof(comment));

			if(!var.compare("deltaT"))			cfgFileStream >> Settings::deltaT;
			if(!var.compare("endTime"))			cfgFileStream >> Settings::endTime;
			if(!var.compare("scenarioType")) 	cfgFileStream >> Settings::scenarioType;
			if(!var.compare("disableOutput")) 	cfgFileStream >> Settings::disableOutput;
			if(!var.compare("inputFile")) 		cfgFileStream >> Settings::inputFile;
			if(!var.compare("epsilon")) 		cfgFileStream >> Settings::epsilon;
			if(!var.compare("sigma")) 			cfgFileStream >> Settings::sigma;
			if(!var.compare("outputFrequency")) cfgFileStream >> Settings::outputFrequency;
			if(!var.compare("loggerConfigFile"))
												cfgFileStream >> Settings::loggerConfigFile;
			if(!var.compare("outputFilePrefix")) cfgFileStream >> Settings::outputFilePrefix;

		}

		cfgFileStream.close();


	}
	else {
		LOG4CXX_ERROR(logger, "Cannot open " << cfgFile);
	}
}

std::string Settings::toString() {
	std::stringstream s;

	s << "\n\tdeltaT = " << Settings::deltaT ;
	s << "\n\tendTime = " << Settings::endTime;
	s << "\n\tscenarioType = " << Settings::scenarioType;
	s << "\n\tcontainerType = " << Settings::containerType;
	s << "\n\tepsilon = " << Settings::epsilon;
	s << "\n\tsigma = " << Settings::sigma;
	s << "\n\tdisableOutput = " << Settings::disableOutput;
	s << "\n\toutputFrequency = " << Settings::outputFrequency;
	s << "\n\tinputFile = " << Settings::inputFile;
	s << "\n\tloggerConfigFile = " << Settings::loggerConfigFile;
	s << "\n\toutputFilePrefix = " << Settings::outputFilePrefix;
	s << "\n\toutputType = " << Settings::outputFileType;
	s << "\n\tdomainSize = " << Settings::domainSize;
	s << "\n\tboundaryConditions:";
	s << "\n\t\tleft:\t"<< Settings::boundaryCondition[0];
	s << "\n\t\tright:\t"<< Settings::boundaryCondition[1];
	s << "\n\t\tbottom:\t"<< Settings::boundaryCondition[2];
	s << "\n\t\ttop:\t"<< Settings::boundaryCondition[3];
	s << "\n\t\tback:\t"<< Settings::boundaryCondition[4];
	s << "\n\t\tfront:\t"<< Settings::boundaryCondition[5];

	s << "\n\trCutoff = " << Settings::rCutoff;

	return s.str();
}
