/*
 * Settings.h
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {
public:
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
