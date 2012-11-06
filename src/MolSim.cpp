#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "utils/Settings.h"
#include "Simulator.h"

#include <cstdlib>
#include <sys/timeb.h>

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int main(int argc, char* argsv[]) {

	if (argc < 2) {
		std::cout << "Erroneous programme call! " << std::endl;
		std::cout << "./molsim filename [optional parameters]" << std::endl;
		std::cout << "Optional parameters are:" << std::endl;
		std::cout << "\t\"deltaT 0.0001\": the desired time step" << std::endl;
		std::cout << "\t\"endTime 100.0\": the end time of simulation" << std::endl;
		std::cout << std::endl;
		std::cout << "EXAMPLE: ./MolSim eingabe_sonne.txt deltaT 0.0014 endTime 100.0" << std::endl;
	}

	Settings::initSettings(argc, argsv);

	Simulator sim;

	double current_time = Settings::startTime;

	int iteration = 0;

	int benchmarkStartTime = getMilliCount();

	 // for this loop, we assume: current x, current f and current v are known
	int maxIterations = (Settings::endTime - Settings::startTime) / Settings::deltaT;
	int iterationsPerDot = maxIterations / 50;
	int nextDotIteration = 1;

	std::cout << "Will calculate " <<  maxIterations << " iterations and output " << maxIterations/Settings::snapshotSkips << " frames " << std::endl;

	while (current_time < Settings::endTime) {
		sim.nextTimeStep();

		iteration++;
		if (!Settings::disableOutput && (iteration % Settings::snapshotSkips == 0)) {
			sim.plotParticles(iteration);
		}
		
		if(iteration >= nextDotIteration) {
			std::cout << "." << std::flush;
			nextDotIteration += iterationsPerDot;
		}
		//std::cout << "Iteration " << iteration << " finished." << std::endl;

		current_time += Settings::deltaT;
	}
	std::cout << std::endl;

	int benchmarkEndTime = getMilliCount();

	std::cout << "Simulation finished. Took " << (benchmarkEndTime - benchmarkStartTime)/1000.0 << " seconds" << std::endl;

	std::cout << "output written. Terminating..." << std::endl;
	return 0;
}

