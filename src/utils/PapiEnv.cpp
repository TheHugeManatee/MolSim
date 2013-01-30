/*
 * PapiEnv.cpp
 *
 *  Created on: 27.01.2013
 *      Author: Leo
 */

#include "PapiEnv.h"

PapiEnv::PapiEnv(std::string name_arg) {
	name = name_arg;
	eventSet = 0;
	realTime = 0;
	procTime = 0;
	flpins = 0;
	mflops = 0;
	totalRealTime = 0;
	totalProcTime = 0;
}

void PapiEnv::flopsStart(){
	int retval;
	PAPI_flops( &realTime, &procTime, &flpins, &mflops);
}

void PapiEnv::flopsEnd(){
	int retval;
	//std::cout << "starting Papi Flops "<< std::endl;
	if((retval=PAPI_flops( &realTime, &procTime, &flpins, &mflops))<PAPI_OK){
		totalRealTime += realTime;
		totalProcTime += procTime;
	}

}

void PapiEnv::reset() {
	totalRealTime = 0;
	totalProcTime = 0;
}
void PapiEnv::printResults(){
	result << totalRealTime << ";" << totalProcTime <<";" << flpins <<";"<< mflops<< ";" << std::endl;
}

void PapiEnv::createResultFile(){
	std::ofstream myfile;
	myfile.open("PapiResults/Papi_"+name);
	myfile << "Real_time;Proc_time;Total flpins;MFLOPS" << std::endl;
	myfile << result.str();
	myfile.close();
}

