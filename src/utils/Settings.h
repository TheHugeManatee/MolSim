/**
 *
 * @file Settings.h
 *
 * @date Oct 30, 2012
 * @author j
 * @class Settings
 *
 * This class manages all global parameters and constants used in the simulation.
 * Any globally availabe setting should be stored here as a public static variable,
 * so it will be accessible in any part of the code.
 *
 * Most of the settings can be specified in a config file. The config file specifies
 * one parameter on every line, the config name being separated from its value by a
 * single space.
 * 
 * When adding new config parameters, the following pieces of code are required:
 * - a new static member variable declaration in the Settings.h header file
 * - the static member definition and an assigned default value in the Settings.cpp file
 * - code to read the value from the input file, for most standard types this will look like
 	\code
	if(!var.compare("newConfigValue")) cfgFile >> Settings::newConfigValue;
	\endcode
 
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_


#include "ScenarioFactory.h"

#include "utils/Vector.h"

#include "simulationConfig.h"
#include <log4cxx/logger.h>

#include <string>

typedef struct {
	double mass;
	double sigma;
	double epsilon;
	bool isMolecule;
	struct  {
		double averageBondLength;
		double stiffness;
		int nX0, nX1, nX2;
	} membraneDescriptor;
} typeDescriptor;

class Settings {
public:

	/**
	 * parses the command line for initialization parameters
	 * @param argc command line parameter count
	 * @param argv parameters for the simulation. Command line Syntax example: deltaT <decimal value> endTime <decimal value>
	 */
	static void initSettings(int argc, char* argv[]);
	/**
	 * starting time of the simulation
	 * typically this should be zero, other values will only make sense if
	 * the input data marks the simulation at this time
	 */
	static double startTime;
	/**
	 * the time the simulation will stop
	 */
	static double endTime;
	/**
	 * the simulation time step delta
	 * the number of calculated time steps will be
	 * (endTime - startTime) / deltaT
	 */
	static double deltaT;

	/**
	 * indicates how many dimensions are use
	 */

	static int dimensions;

	/**
	 * path to the config file
	 */
	static std::string configFile;

	/**
	 * how many simulation step should be calculated between two output frames
	 */
	static int outputFrequency;

	/**
	 * the type of simulation scenario
	 * @see ScenarioFactory for available types
	 */
	static ScenarioType scenarioType;

	/**
	 * disable the file output
	 */
	static bool disableOutput;

	/**
	 * the input file describing the startup scenario
	 */
	static std::string inputFile;

	/**
	 * implies whether the last state is saved or not (.present())
	 * if it is, a file path is given (.get())
	 */
	static bool saveLastState;
	static std::string lastStateFile;

	/*
	 * sigma parameter for the Lennard-Jones potential
	 */
	//static double sigma;

	/*
	 * epsilon parameter for the Lennard-Jones potential
	 */
	//static double epsilon;

	/**
	 * gravitation constant for the gravitational force
	 */
	static utils::Vector<double,3> gravitation;

	static SimulationConfig::ForceFieldSequence forceFields;
	/**
	 * the test case to be executed
	 */
	static std::string testCase;

	/**
	 * the logger configuration file
	 */
	static std::string loggerConfigFile;

	/**
	 * the prefix of all output files
	 * can include folders, but they will have to exist already
	 */
	static std::string outputFilePrefix;


	/**
	 * number of defined particle types
	 */
	static int numParticleTypes;

	/**
	 * all available particle types
	 */
	static typeDescriptor *particleTypes;

	/**
	 * pre calculated epsilon geometric mean values
	 */
	static double *geometricMeanEpsilon;

	/**
	 * generator configuration
	 */
	static SimulationConfig::GeneratorType generator;

	/*
	 * thermostat switch (implies whether the thermostat is on or not)
	 */
	static SimulationConfig::ThermostatSwitchType thermostatSwitch;

	/**
	 * thermostat configuration
	 */
	static SimulationConfig::ThermostatSettingsType *thermostatSettings;

	/**
	 * the cutoff radius for the simulation
	 */
	static double rCutoff;


	/**
	 * the size of the simulation domain
	 * the domain will start at (0,0,0) and have the size of domainSize
	 */
	static utils::Vector<double, 3> domainSize;

	/**
	 * the container type to be used
	 * either standard ParticleContainer or CelllistContainer
	 */
	static ContainerType containerType;

	/**
	 * the output file type
	 * xyz or vtk
	 */
	static OutputFileType outputFileType;

	/**
	 * the boundary conditions
	 * for every boundary, a different condition can be specified
	 * [0] ~ left
	 * [1] ~ right
	 * [2] ~ bottom
	 * [3] ~ top
	 * [4] ~ back
	 * [5] ~ front
	 */
	static BoundaryConditionType boundaryCondition[6];

	/**
	 * open an interactive 3d view of the last rendered frame
	 */
	static bool show3DVisual;

	/**
	 * CellListContainer changes the type for each cell to visualize the particles belonging to same cells
	 */
	static bool encodeCellsInType;


private:
	/**
	 * loads config parameters from a config file
	 * @param cfgFile path to the config file
	 */
	static void parseCfgFile(std::string cfgFile);

	static void parseXmlFile(std::string xmlFile);

	/**
	 * the internal logger instance for logging settings-related things
	 */
	static log4cxx::LoggerPtr logger;

	static std::string toString();
};

#endif /* SETTINGS_H_ */
