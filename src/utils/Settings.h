/**
 *
 * static class containing all global parameters needed for simulation
 *
 *
 * @file Settings.h
 *
 * @date Oct 30, 2012
 * @author j
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>

class Settings {
public:

	/**
	 * parses the command line for initialization parameters
	 * @param argc command line parameter count
	 * @param argv parameters for the simulation. Syntax: deltaT <decimal value> endTime <decimal value>
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
	 * path to the config file
	 */
	static std::string configFile;

	/**
	 * how many simulation step should be calculated between two output frames
	 */
	static int snapshotSkips;

	/**
	 * the type of simulation scenario
	 * @see ScenarioFactory for available types
	 */
	static std::string scenarioType;

	/**
	 * disable the file output
	 */
	static bool disableOutput;

	/**
	 * the input file describing the startup scenario
	 */
	static std::string inputFile;

	static double sigma;
	static double epsilon;

private:
	static void loadConfigFile(std::string cfgFile);
};

#endif /* SETTINGS_H_ */
