/*
 * PapiEnv.h
 *
 *  Created on: 27.01.2013
 *      Author: Leo
 */

#ifndef PAPIENV_H_
#define PAPIENV_H_

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <papi.h>


class PapiEnv {
public:
	int* eventSet;
	float realTime;
	float procTime;
	long long int flpins;
	float mflops;
	std::string name;
	std::stringstream result;

	PapiEnv(std::string name_arg);
	void papiFlops();
	void printResults();
	void createResultFile();
};

#endif /* PAPIENV_H_ */
