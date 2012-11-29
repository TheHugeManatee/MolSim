/**
 * @mainpage
 *
 * @section Authors
 * 		Leonhard Rannabauer
 *
 * 		Jakob Weiss
 *
 * 		Alexander Winkler
 *
 *
 * @section Purpose
 * 		molecular dynamics simulation for approximating the behavior of arbitrary systems composed of molecules
 *
 * @section usage Usage
 * 		The program does not have any mandatory command line parameters. If it is run without parameters, it will look for
 * 		a file named <tt>simulationConfig.xml</tt> in the CWD. This will be used as the configuration file to set up the
 * 		simulation.
 *
 * 		All parameters specified on the command line will override the specifications of the config file.
 *
 * @subsection params Command Line Parameters
 *
 * @subsection cfg Config Files
 *
 * @subsection input Input Files
 *
 */

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

/**
 * set up the logger based on Settings::loggerConfigFile
 */
void initializeLogger();
/**
 * print out a progress bar on the console command line
 *
 * @param percentage the percentage finished of the job, should be in the interval [0;100]
 * @param elapsed time it took to compute the current progress, measured in milliseconds
 */
void printProgressBar(int percentage, int elapsed);


//globals
auto rootLogger = log4cxx::Logger::getRootLogger();


int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int main(int argc, char* argsv[]) {
	if (argc > 1 && (!strcmp(argsv[1], "-?") || !strcmp(argsv[1], "help") || !strcmp(argsv[1], "--help"))) {
		std::cout << "This is the NUKULAR Simulator" << std::endl;
		std::cout << "Authors: " << std::endl;
		std::cout << "\tLeonhard Rannabauer" << std::endl;
		std::cout << "\tJakob Weiss" << std::endl;
		std::cout << "\tAlexander Winkler" << std::endl;
		std::cout << std::endl;
		std::cout << "By default, configuration will be loaded from a configuration file." << std::endl;
		std::cout << "The CWD will be searched for \"config.cfg\". Override this by specifying the" << std::endl;
		std::cout << "\t-configFile command line parameter with the path to your own configuration" << std::endl;
		std::cout << "\t(either .xml or .cfg format)" << std::endl;
		std::cout << std::endl;
		std::cout << "EXAMPLE: ./MolSim -configFile simulationConfig.xml -outputFilePrefix MD_sim -endTime 10" << std::endl;
		std::cout << std::endl;
		std::cout << "\t will load Settings from simulationConfig.xml, simulate the world for 10 seconds" << std::endl;
		std::cout << "\t and output the files with a prefix of \"MD_sim\", which will lead to files like" << std::endl;
		std::cout << "\t\"MD_sim_0010.vtu\"" << std::endl;
		return 0;
	}

	std::cout << "Initializing the logger..." << std::endl << std::flush;
	//Initialize the logging stuff
	initializeLogger();


	Settings::initSettings(argc, argsv);
	LOG4CXX_TRACE(rootLogger, "Settings initialized!");

	//Check if we should be executing some unit tests
	if(!Settings::testCase.empty()) {
		return executeTests();
	}

	LOG4CXX_TRACE(rootLogger, "Creating Simulator instance...");
	Simulator *sim = new Simulator();

	double current_time = Settings::startTime;

	int iteration = 0;

	int benchmarkStartTime = getMilliCount();

	 // for this loop, we assume: current x, current f and current v are known
	int maxIterations = (Settings::endTime - Settings::startTime) / Settings::deltaT;
	int nextProgressBarDraw = 1;
	int iterationsPerPercent = (maxIterations/100) + 1;

	LOG4CXX_INFO(rootLogger, "Will calculate " <<  maxIterations << " iterations and output " << maxIterations/Settings::outputFrequency << " frames ");

	while (current_time < Settings::endTime) {
		if (!Settings::disableOutput && (iteration % Settings::outputFrequency == 0)) {
			sim->plotParticles(iteration);
		}

		sim->nextTimeStep();

		iteration++;
		
		if(iteration == nextProgressBarDraw) {
			nextProgressBarDraw+=iterationsPerPercent;
			printProgressBar(100*iteration/maxIterations, -(benchmarkStartTime - getMilliCount()));
		}

		LOG4CXX_TRACE(rootLogger, "Iteration " << iteration << " finished.");

		current_time += Settings::deltaT;
	}
	std::cout << std::endl;

	int benchmarkEndTime = getMilliCount();

	LOG4CXX_INFO(rootLogger, "Simulation finished. Took " << (benchmarkEndTime - benchmarkStartTime)/1000.0 << " seconds");

	delete sim;
	
	LOG4CXX_DEBUG(rootLogger, "Created " << Particle::createdInstances << " Particle instances (" << Particle::createdByCopy << " by copy)");
	LOG4CXX_DEBUG(rootLogger, "Destroyed " << Particle::destroyedInstances << " Particle instances");

	//10 is arbitrarily chosen. there will always be some stray particles because of
	//static instances that will be destroyed at program exit
	if(Particle::createdInstances - Particle::destroyedInstances > 10) {
		LOG4CXX_WARN(rootLogger, "Significant mismatch between created and destroyed particle instances. This can be a memory leak!" << (Particle::createdInstances - Particle::destroyedInstances));
	}

	LOG4CXX_DEBUG(rootLogger, "output written. Terminating...");
	return 0;
}

/**
 * execute a specific or all test cases, depending on the Settings::testCase parameter
 */
int executeTests() {
	std::cout << "Running tests..." << std::endl;


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
	  std::cout << "Tests ok!" << std::endl;
	}
	else {
		std::cout << "Some tests failed!" << std::endl;
	}

	return wasSuccessful ? 0 : 1;
}


void initializeLogger() {
    if (!Settings::loggerConfigFile.empty()) {
            log4cxx::PropertyConfigurator::configure(Settings::loggerConfigFile);
    }
    else {
            log4cxx::BasicConfigurator::configure();
    }
}

void printProgressBar(int percentage, int elapsed){
	std::cout << "[";
	int i = 0;
	for(; i<percentage; i=i+3){
		std::cout << "=";
	}
	for(; i<100;i=i+3){
		std::cout << " ";
	}
	std::cout << "]";
	if(percentage !=0){
		int estSeconds = (elapsed/percentage)/10 - elapsed/1000,
				estMinutes = 0, estHours = 0, estDays = 0;

		if(estSeconds > 60) {
			estMinutes = estSeconds / 60;
			estSeconds = estSeconds % 60;
		}
		if(estMinutes > 60) {
			estHours = estMinutes / 60;
			estMinutes = estMinutes % 60;
		}
		if(estHours > 24) {
			estDays = estHours / 24;
			estHours = estHours % 60;
		}
		std::cout << " " << percentage << "%  Est. Remaining: ";
		if(estDays) std::cout << estDays << "d ";
		if(estHours) std::cout << estHours << "h ";
		if(estMinutes) std::cout << estMinutes << "m ";
		std::cout << estSeconds <<"s    \r";
	}
	else{
		std::cout << " " << percentage << "%  Est. Remaining: TBD\r";
	}
	std::cout.flush();
}
