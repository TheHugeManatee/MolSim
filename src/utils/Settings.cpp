/*
 * Settings.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: Jakob Weiss
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

#include "outputWriter/RenderOutputWriter.h"

namespace outputWriter {
	extern double primitiveScaling;
	extern bool renderHalo;
	extern bool renderMembrane;
	extern bool renderingPaused;
	extern bool renderCells;
	extern bool renderFilledCells;
	extern bool editMode;
	extern double camPosition[3];
	extern double camRotation[3];
	extern int currentColoring;
	extern int currentScale;
}

int Settings::numThreads = 0;

//variable definitions and  default settings
double Settings::deltaT = 0.0014;
double Settings::startTime = 0;
double Settings::endTime = 100.0;
int Settings::dimensions = 3;
int Settings::outputFrequency = 10;
bool Settings::disableOutput = false;
//double Settings::sigma = 1;
//double Settings::epsilon = 5;

utils::Vector<double,3> Settings::gravitation = 0.0;

std::vector<ForceFieldDescriptor> Settings::forceFields;
ScenarioType Settings::scenarioType = ScenarioType::Gravity;
std::string Settings::configFile = "simulationConfig.xml";
std::string Settings::inputFile = "";
bool Settings::saveLastState = "false";
std::string Settings::lastStateFile = "ausgabe.txt";

bool Settings::printStatistics = "false";
std::string Settings::statisticsFile = "";
int Settings::statisticsInterval = 0;
double Settings::deltaRDF = 0;

std::string Settings::testCase = "";
std::string Settings::loggerConfigFile = "";
std::string Settings::outputFilePrefix = "OutputFiles/MD_vtk_";
int Settings::outputFileIterationOffset = 0;
OutputFileType Settings::outputFileType = OutputFileType::vtk;
double Settings::rCutoff = 3;
double Settings::rl = 1.9;
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


typeDescriptor *Settings::particleTypes = NULL;
double *Settings::geometricMeanEpsilon = NULL;
int Settings::numParticleTypes = 0;
SimulationConfig::GeneratorType Settings::generator;

SimulationConfig::ThermostatSwitchType Settings::thermostatSwitch = SimulationConfig::ThermostatSwitchType::OFF;
SimulationConfig::ThermostatSettingsType *Settings::thermostatSettings = new ThermostatSettings(0);

log4cxx::LoggerPtr Settings::logger = log4cxx::Logger::getLogger("Settings");

/**
 * static class containing all global settings relevant to simulation and
 * material etc.
 * parameters can be configured through the command line and through a config
 * standard config filename is config.cfg
 */
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
		if(strcmp(argv[i], "-outputFrequency") == 0 && argc > i + 1)
			Settings::outputFrequency = atof(argv[i+1]);
		if(strcmp(argv[i], "-visualize") == 0)
			Settings::show3DVisual = true;
		if(strcmp(argv[i], "-colorByCell") == 0)
			Settings::encodeCellsInType = true;
		if(strcmp(argv[i], "-outputFileIterationOffset") == 0 && argc > i + 1)
			Settings::outputFileIterationOffset = atoi(argv[i+1]);
		if(strcmp(argv[i], "-numThreads") == 0 && argc > i + 1)
			Settings::numThreads = atoi(argv[i+1]);

	}

	//Re-initialize the logger with possibly new configuration file
	initializeLogger();

	LOG4CXX_DEBUG(logger, "Configuration loaded: " << Settings::toString());

	assert(Settings::deltaT != 0);//timestep needs to be non-zero

	assert(sgn(Settings::deltaT) == sgn(Settings::endTime));	//deltaT and endTime have the same sign, otherwise the computation will never end
}


void Settings::parseXmlFile(std::string cfgFile) {
	LOG4CXX_DEBUG(logger, "Loading Configuration from .xml file " << cfgFile);
	 try {
	    std::auto_ptr<SimulationConfig> xmlCfg (simulationConfig(cfgFile, xml_schema::Flags::dont_validate));

	    Settings::deltaT = xmlCfg->deltaT();
	    Settings::endTime = xmlCfg->endTime();
	    auto dimensions_opt = xmlCfg->dimensions();
	    if(dimensions_opt.present())
	    	Settings::dimensions = dimensions_opt.get();
	    Settings::outputFrequency = xmlCfg->outputFrequency();
	    Settings::disableOutput = xmlCfg->disableOutput();
	    Settings::loggerConfigFile = xmlCfg->loggerConfigFile();
	    Settings::outputFilePrefix = xmlCfg->outputFilePrefix();
	    Settings::outputFileIterationOffset = xmlCfg->outputFileIterationOffset().present()?xmlCfg->outputFileIterationOffset().get():0;
	    Settings::scenarioType = xmlCfg->scenarioType();
	    auto inputFile_opt = xmlCfg->inputFile();
	    if(inputFile_opt.present())
	    	Settings::inputFile = inputFile_opt.get();
	    auto lastState_opt = xmlCfg->saveLastState();
	    Settings::saveLastState = lastState_opt.present();
	    if(Settings::saveLastState){
	    	Settings::lastStateFile = lastState_opt.get();
	    	LOG4CXX_INFO(logger,"Will save last state to "<<Settings::lastStateFile);
	    }else{
	    	LOG4CXX_DEBUG(logger,"No file for last state given");
	    }

	    auto printStatistics_opt = xmlCfg->statistics();
	    Settings::printStatistics = printStatistics_opt.present();
	    if(Settings::printStatistics){
	    	Settings::statisticsFile = printStatistics_opt.get().fileName();
	    	Settings::statisticsInterval = printStatistics_opt.get().interval();
	    	Settings::deltaRDF = printStatistics_opt.get().deltaR();
	    	LOG4CXX_INFO(logger, "Saving statistics to " << Settings::statisticsFile <<" every "<< Settings::statisticsInterval <<" for Radius: " << Settings::deltaRDF);
	    }

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
	    auto cutOffRadius_arg = xmlCfg->cutoffRadius();


	    double max_sigma = 0;

	    Settings::numParticleTypes = xmlCfg->typeList().particleType().size();
	    Settings::particleTypes = new typeDescriptor[Settings::numParticleTypes];
	    for(int i=0; i < xmlCfg->typeList().particleType().size(); i++) {
	    	Settings::particleTypes[i].sigma = xmlCfg->typeList().particleType()[i].sigma();
	    	Settings::particleTypes[i].epsilon = xmlCfg->typeList().particleType()[i].epsilon();
	    	Settings::particleTypes[i].mass = xmlCfg->typeList().particleType()[i].mass();
	    	Settings::particleTypes[i].isMolecule = xmlCfg->typeList().particleType()[i].isMolecule();
	    	Settings::particleTypes[i].membraneDescriptor.stiffness = xmlCfg->typeList().particleType()[i].stiffness();
	    	Settings::particleTypes[i].membraneDescriptor.averageBondLength = xmlCfg->typeList().particleType()[i].averageBondLength();

	    	Settings::particleTypes[i].membraneDescriptor.nX0 = xmlCfg->typeList().particleType()[i].nX0();
	    	Settings::particleTypes[i].membraneDescriptor.nX1 = xmlCfg->typeList().particleType()[i].nX1();
	    	Settings::particleTypes[i].membraneDescriptor.nX2 = xmlCfg->typeList().particleType()[i].nX2();


	    	if(Settings::particleTypes[i].sigma > max_sigma)
	    		max_sigma = Settings::particleTypes[i].sigma;
	    }

	    Settings::geometricMeanEpsilon = new double[Settings::numParticleTypes * Settings::numParticleTypes];

	    for(int i = 0; i<Settings::numParticleTypes ; i++)
	    	for(int j = i; j < Settings::numParticleTypes ; j++){
	    		Settings::geometricMeanEpsilon[i+Settings::numParticleTypes*j] = sqrt(Settings::particleTypes[i].epsilon * Settings::particleTypes[j].epsilon);
	    		Settings::geometricMeanEpsilon[j+Settings::numParticleTypes*i] = sqrt(Settings::particleTypes[i].epsilon * Settings::particleTypes[j].epsilon);
	    	}

	    Settings::rCutoff = cutOffRadius_arg.radius();
	    if(cutOffRadius_arg.scaled()){
	    	Settings::rCutoff = Settings::rCutoff * max_sigma;
	    	LOG4CXX_INFO(logger,"CutOff radius is scaled by " << max_sigma);
	    }

	    auto lRadius_opt = xmlCfg->lRadius();
	    if(lRadius_opt.present()){
	    	Settings::rl = lRadius_opt.get();
	    }

	    Settings::generator = xmlCfg->generator();

	    auto thermostatSwitch_opt = xmlCfg->thermostatSwitch();
	    if (thermostatSwitch_opt.present())
	    	Settings::thermostatSwitch = thermostatSwitch_opt.get();
	    if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
	    	auto thermostatSetting_opt = xmlCfg->thermostatSettings();
	    	if(thermostatSetting_opt.present()){
	    	LOG4CXX_DEBUG(logger,"Thermostat is ON , loading Thermostat Settings ...");
	    	*Settings::thermostatSettings = thermostatSetting_opt.get();
	    	LOG4CXX_DEBUG(logger,"Loaded Thermostat Settings ...");
	    	}
	    }

	    //Settings::epsilon = xmlCfg->epsilon();
	    //Settings::sigma = xmlCfg->sigma();
	    auto gravationalConstant_opt = xmlCfg->gravitation();

	    if(gravationalConstant_opt.present()){
	    	Settings::gravitation[0] = gravationalConstant_opt.get().x0();
	    	Settings::gravitation[1] = gravationalConstant_opt.get().x1();
	    	Settings::gravitation[2] = gravationalConstant_opt.get().x2();
	    }

	    //Settings::forceFields = xmlCfg->forceField();
	    ForceFieldDescriptor field;
	    for(int i = 0 ; i< xmlCfg->forceField().size() ; i++){
	    	auto f = xmlCfg->forceField()[i];

	    	field.type = f.type();
	    	field.from[0] = f.from().x0();
	    	field.from[1] = f.from().x1();
	    	field.from[2] = f.from().x2();
	    	field.to[0] = f.to().x0();
	    	field.to[1] = f.to().x1();
	    	field.to[2] = f.to().x2();
	    	field.force[0] = f.force().x0();
	    	field.force[1] = f.force().x1();
	    	field.force[2] = f.force().x2();
	    	field.startTime = f.startTime().present()?f.startTime().get():0;
	    	field.endTime = f.endTime().present()?f.endTime().get():Settings::endTime;
	    	Settings::forceFields.push_back(field);
		}

	    auto rcfg = xmlCfg->renderConfig();
	    if(rcfg.present()) {
	    	auto renderCfg = rcfg.get();
	    	if(renderCfg.enabled().present()) Settings::show3DVisual = renderCfg.enabled();
	    	if(renderCfg.showCells().present()) outputWriter::renderFilledCells = renderCfg.showCells().get();
	    	if(renderCfg.showMembrane().present()) outputWriter::renderMembrane = renderCfg.showMembrane().get();
	    	if(renderCfg.coloring().present()) outputWriter::currentColoring = renderCfg.coloring().get();
	    	if(renderCfg.scale().present()) outputWriter::currentScale = renderCfg.scale().get();
	    	if(renderCfg.primitiveScaling().present()) outputWriter::primitiveScaling = renderCfg.primitiveScaling().get();
	    	if(renderCfg.camPosition().present()) {
	    		outputWriter::camPosition[0] = renderCfg.camPosition().get().x0();
	    		outputWriter::camPosition[1] = renderCfg.camPosition().get().x1();
	    		outputWriter::camPosition[2] = renderCfg.camPosition().get().x2();
	    	}
	    	if(renderCfg.camRotation().present()) {
	    		outputWriter::camRotation[0] = renderCfg.camRotation().get().x0();
	    		outputWriter::camRotation[1] = renderCfg.camRotation().get().x1();
	    		outputWriter::camRotation[2] = renderCfg.camRotation().get().x2();
	    	}
	    }

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
			"New features will likely be implemented in the xml reader only!");

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

