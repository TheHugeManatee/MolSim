#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <sys/timeb.h>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCaller.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>


#include "UnitTests/ParticleContainerTests.h"

#include "utils/Settings.h"
#include "Simulator.h"

//Forward declarations
int executeTests();
void initializeLogger();


//globals
auto rootLogger = log4cxx::Logger::getRootLogger();


int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int main(int argc, char* argsv[]) {
	if (argc < 2) {
		std::cout << "Erroneous program call! " << std::endl;
		std::cout << "./molsim [optional parameters]" << std::endl;
		std::cout << "Optional parameters are:" << std::endl;
		std::cout << "\t\"deltaT 0.0001\": the desired time step" << std::endl;
		std::cout << "\t\"endTime 100.0\": the end time of simulation" << std::endl;
		std::cout << std::endl;
		std::cout << "EXAMPLE: ./MolSim eingabe_sonne.txt deltaT 0.0014 endTime 100.0" << std::endl;
	}

	Settings::initSettings(argc, argsv);

	//Initialize the logging stuff
	initializeLogger();

	//Check if we should be executing some unit tests
	if(!Settings::testCase.empty()) {
		return executeTests();
	}

	Simulator sim;

	double current_time = Settings::startTime;

	int iteration = 0;

	int benchmarkStartTime = getMilliCount();

	 // for this loop, we assume: current x, current f and current v are known
	int maxIterations = (Settings::endTime - Settings::startTime) / Settings::deltaT;
	int iterationsPerDot = maxIterations / 50;
	int nextDotIteration = 1;

	LOG4CXX_INFO(rootLogger, "Will calculate " <<  maxIterations << " iterations and output " << maxIterations/Settings::snapshotSkips << " frames ");

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
		LOG4CXX_TRACE(rootLogger, "Iteration " << iteration << " finished.");

		current_time += Settings::deltaT;
	}
	std::cout << std::endl;

	int benchmarkEndTime = getMilliCount();

	LOG4CXX_INFO(rootLogger, "Simulation finished. Took " << (benchmarkEndTime - benchmarkStartTime)/1000.0 << " seconds");
	
	LOG4CXX_DEBUG(rootLogger, "Created " << Particle::createdInstances << " Particle instances (" << Particle::createdByCopy << " by copy)");
	LOG4CXX_DEBUG(rootLogger, "Destroyed " << Particle::destroyedInstances << " Particle instances");
	LOG4CXX_DEBUG(rootLogger, "output written. Terminating...");
	return 0;
}

/**
 * execute a specific or all test cases, depending on the Settings::testCase parameter
 */
int executeTests() {

	  CppUnit::TextUi::TestRunner runner;

	  bool all = !Settings::testCase.compare("all");

	  if(all || !Settings::testCase.compare("ParticleContainer"))
		  runner.addTest(ParticleContainerTests::suite());


	  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
	                                                       std::cerr ) );
	  // Run the tests.
	  bool wasSuccessful = runner.run();

	  // Return error code 1 if the one of test failed.
	  if(wasSuccessful) {
		  std::cout << "Tests ok!";
	  }

	  return wasSuccessful ? 0 : 1;
}


void initializeLogger() {
    if (!Settings::loggerConfigFile.empty())
    {
            log4cxx::PropertyConfigurator::configure(Settings::loggerConfigFile);
    }
    else
    {
            log4cxx::BasicConfigurator::configure();
    }
}
