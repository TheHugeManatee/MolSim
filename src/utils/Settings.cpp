/*
 * Settings.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#include <stdlib.h>
#include <string.h>

#include "Settings.h"

double Settings::deltaT = 0.0014;
double Settings::startTime = 0;
double Settings::endTime = 100.0;

static void Settings::initSettings(int argc, char* argv[]) {

	for(int i=0; i < argc; i++) {
		if(strcmp(argv[i], "deltaT") == 0 && argc > i + 1)
			Settings::deltaT = atof(argv[i+1]);
		if(strcmp(argv[i], "endTime") == 0 && argc > i + 1)
					Settings::endTime = atof(argv[i+1]);

	}
}

static double Settings::getStartTime() {
	return Settings::startTime;
}

static double Settings::getEndTime() {
	return Settings::endTime;
}

static double Settings::getDeltaT() {
	return Settings::deltaT;
}

