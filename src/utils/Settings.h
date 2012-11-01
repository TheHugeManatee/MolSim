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

class Settings {
public:

	/**
	 * parses the command line for initialization parameters
	 * @param argc command line parameter count
	 * @param argv parameters for the simulation. Syntax: deltaT <decimal value> endTime <decimal value>
	 */


	static void initSettings(int argc, char* argv[]);
	static double getStartTime();
	static double getEndTime();
	static double getDeltaT();

private:
	static double startTime;
	static double endTime;
	static double deltaT;

};

#endif /* SETTINGS_H_ */
