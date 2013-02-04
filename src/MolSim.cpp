/**
 * @mainpage
 *
 * @section Authors
 * Leonhard Rannabauer
 *
 * Jakob Weiss
 *
 * Alexander Winkler
 *
 *
 * @section Purpose
 * molecular dynamics simulation for approximating the behavior of arbitrary systems composed of molecules
 *
 * @section usage Usage
 * The program does not have any mandatory command line parameters. If it is run without parameters, it will look for
 * a file named <tt>simulationConfig.xml</tt> in the CWD. This will be used as the configuration file to set up the
 * simulation.
 *
 *  All parameters specified on the command line will override the specifications of the config file.
 *
 * @section build Building
 * Building should happen in a unix environment via the Makefile provided in the root directory.
 *
 * To build the project, currently the following is required:
 *
 * - Library log4cxx Logging framework [0.10.0-11]
 * - Library xercesc xml parsing library [3.0.1-10]
 * - Library cppunit for unit tests [1.12.1-2]
 * - Codesynthesis xsd tool [3.3.0]
 * - *Library freeglut for OpenGL Rendering [2.8.0]
 * 		If you are not able to install the freeglut library, you can use the Makefile_NoGLUT. This will build
 * 		the Project without the OpenGL live renderer to remove the dependency to OpenGL and freeglut
 *
 * @note The Makefile automatically builds the utils/simulationConfig.cpp and .h files by using the codesynthesis
 * 	xsd converter.
 *
 *
 * @subsection params Command Line Parameters
 * certain parameters can be specified via command line to override the builtin defaults and the values given in the
 * configuration file
 *
 * available parameters are:
 *
 * - <tt>-configFile <pathToFile></tt>: path to a custom configuration file (xml or cfg)
 *
 * - <tt>-inputFile <pathToFile></tt>: path to a custom input file
 *
 * - <tt>-endTime <double> </tt>: simulation end time
 *
 * - <tt>-disableOutput: explicitly disable output (for benchmarking)
 *
 * - <tt>-outputFilePrefix <pathAndFilePrefix></tt>: the file prefix for output files
 *
 * - <tt>-test <testName></tt>: run specific unit test ("all" to run all available unit tests)
 *
 * - <tt>-visualize</tt>: show a live rendering of the current simulator state
 *
 * @subsection cfg Config Files
 * Config files can have either .cfg or .xml format
 * @subsubsection cfgcfg .cfg Format
 * The .cfg format is a simple line-based format to specify configuration parameters.
 * Lines beginning with "# " will be ignored as comments.
 * Every non-comment-line first specifies the name of the config parameter and then, separated
 * by a space, the value to be assigned to that parameter.
 *
 * @warning the .cfg Format is obsolete and usage is discouraged. .xml format offers more advanced options becuase
 * the parser for .cfg Format has not been updated to read new config parameters introduced in more recent versions.
 *
 * @subsubsection cfgxml .xml Format
 * The XML config file format allows to specify Settings in a easily readable, strictly well-formed format. The parser is
 * generated from the simulationConfig.xsd schema file by the Codesynthesis XSD tool. This means that the XML configuration
 * file has to fulfill the XML Schema Definitions in simulationConfig.xsd. Use simulationConfig.xml as a starting point to
 * build your own configurations.
 *
 * @subsection input Input Files
 * Input files contain particle definition data. The first lines starting with "#" will be ignored. The first non-comment line
 * specifies the number of explicit particle definitions in the file.
 *
 * After this line, as many lines are expected, each defining a particle by position, type, mass and velocity.
 *
 * After the explicit particle definitions, there can be arbitrarily many cuboid definitions. Since cuboids are easier defined
 * in the XML Config files, that should be the preferred way to do it. Cuboid functionality in the input files is only
 * kept for backwards compatibility.
 *
 */

#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <sys/timeb.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCaller.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "UnitTests/ParticleContainerTests.h"
#include "UnitTests/ParticleGeneratorTests.h"
#include "UnitTests/SettingsXsdTest.h"
#include "UnitTests/MatrixTests.h"

#include "utils/Settings.h"
#include "Simulator.h"
#include "utils/ThermostatDiscrete.h"

#ifndef NOGLVISUALIZER
namespace outputWriter {
extern std::vector<Particle> render3dParticles;
extern bool renderingPaused;
extern Simulator *theSimulator;
}
#endif

#ifdef PAPI_BENCH
#include "utils/PapiEnv.h"
PapiEnv *papiCalcFCounters[8];
PapiEnv *papiCalcXCounters[8];
#endif
//Forward declarations


 /**
  * @brief  execute a specific or all test cases, depending on the Settings::testCase parameter
  *
  *         Runs all test that are specified in Settings parameter
  *
  * @return status code: 0 if all tests were successful
  *
  */
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

/**
 * gets System time in milliseconds
 *
 * @return System time in milliseconds
 */
int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

/**
 * Main method (Duh ;) ) Runs the whole simulation. Main loop runs in this method
 *
 * @param argc number of command line arguments
 * @param argsv[] command line parameters. Several Config file parameters can be overriden by setting them here. "--visualize" is especially cool, as it shows an OpenGL live view
 *
 * @return 0 if successful
 */
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

#ifdef _OPENMP
	if(Settings::numThreads > 0) {
		LOG4CXX_INFO(rootLogger, "Setting OpenMP Threads to " << Settings::numThreads);
		omp_set_num_threads(Settings::numThreads);
	}
	else {
		LOG4CXX_INFO(rootLogger, "Running on " << omp_get_max_threads() << " threads");
		Settings::numThreads = omp_get_max_threads();
	}
#else
	Settings::numThreads = 1;
	LOG4CXX_INFO(rootLogger, "Running serial version!");
#endif

#ifdef PAPI_BENCH
	for(int i=0; i < Settings::numThreads; i++) {
		char fileName[200];
		sprintf(fileName, "CalcF #%i.txt", i);
		papiCalcFCounters[i] = new PapiEnv(fileName);
		sprintf(fileName, "CalcX #%i.txt", i);
		papiCalcXCounters[i] = new PapiEnv(fileName);
	}	
#endif
	//Check if we should be executing some unit tests
	if(!Settings::testCase.empty()) {
		return executeTests();
	}

	LOG4CXX_TRACE(rootLogger, "Creating Simulator instance...");
	Simulator *sim = new Simulator();
#ifndef NOGLVISUALIZER
	outputWriter::theSimulator = sim;
#endif
	//Check if we should initialize with old state file
	if(Settings::inputFile.size() !=0){
		std::cout << "state found"<<std::endl;
	}



	double current_time = Settings::startTime;

	int iteration = 0;

	int benchmarkStartTime = getMilliCount();
	double timeForOneIteration = 0;

	 // for this loop, we assume: current x, current f and current v are known
	int maxIterations = (Settings::endTime - Settings::startTime) / Settings::deltaT;
	int nextProgressBarDraw = 1;
	int iterationsPerPercent = (maxIterations/100) + 1;

	LOG4CXX_INFO(rootLogger, "Will calculate " <<  maxIterations << " iterations and output " << maxIterations/Settings::outputFrequency << " frames ");


	while (current_time < Settings::endTime) {
		if (iteration % Settings::outputFrequency == 0) {
			sim->plotParticles(iteration + Settings::outputFileIterationOffset);
		}
		sim->nextTimeStep();
		iteration++;
		if(iteration == nextProgressBarDraw) {
			nextProgressBarDraw+=iterationsPerPercent;
			printProgressBar(100*iteration/maxIterations, -(benchmarkStartTime - getMilliCount()));
		}
		LOG4CXX_TRACE(rootLogger, "Iteration " << iteration << " finished.");
		current_time += Settings::deltaT;
		timeForOneIteration = ((double)(benchmarkStartTime - getMilliCount()))/iteration;

#ifndef NOGLVISUALIZER
		while(outputWriter::renderingPaused)		usleep(2000);
#endif

#ifdef PAPI_BENCH
		for(int i=0; i < Settings::numThreads; i++) {
			papiCalcFCounters[i]->printResults();
			papiCalcXCounters[i]->printResults();
			papiCalcFCounters[i]->reset();
			papiCalcXCounters[i]->reset();
		}
#endif
	}


	int benchmarkEndTime = getMilliCount();

	if(Settings::saveLastState)
		sim->exportPhaseSpace();
	if(Settings::printStatistics)
		sim->exportStatistics();

	LOG4CXX_INFO(rootLogger, "Simulation finished. Took " << (benchmarkEndTime - benchmarkStartTime)/1000.0 << " seconds");

	delete sim;


	LOG4CXX_DEBUG(rootLogger, "Created " << Particle::createdInstances << " Particle instances (" << Particle::createdByCopy << " by copy)");
	LOG4CXX_DEBUG(rootLogger, "Destroyed " << Particle::destroyedInstances << " Particle instances");

#ifdef PAPI_BENCH
	
	for(int i=0; i < Settings::numThreads; i++) {
		std::cout << "Writing PAPI output for thread " << i << std::endl;
		papiCalcFCounters[i]->createResultFile();
		papiCalcXCounters[i]->createResultFile();
		delete 	papiCalcFCounters[i];
		delete papiCalcXCounters[i];
	}	
#endif
	//10 is arbitrarily chosen. there will always be some stray particles because of
	//static instances that will be destroyed at program exit
#ifndef NOGLVISUALIZER
	if(Particle::createdInstances - Particle::destroyedInstances - outputWriter::render3dParticles.size() > 10) {
#else
	if(Particle::createdInstances - Particle::destroyedInstances > 10) {
#endif
		LOG4CXX_WARN(rootLogger, "Significant mismatch between created and destroyed particle instances. This can be a memory leak! " << (Particle::createdInstances - Particle::destroyedInstances));
	}

	LOG4CXX_DEBUG(rootLogger, "output written. Terminating...");
	return 0;
}


int executeTests() {
	std::cout << "Running tests..." << std::endl;

	CppUnit::TextUi::TestRunner runner;

	bool all = !Settings::testCase.compare("all");

	if(all || !Settings::testCase.compare("ParticleContainer"))
	  runner.addTest(ParticleContainerTests::suite());

	if(all || !Settings::testCase.compare("ParticleGenerator"))
		 runner.addTest(ParticleGeneratorTests::suite());

//	if(all || !Settings::testCase.compare("Settings"))
//		 runner.addTest(SettingsXsdTest::suite());


	if(all || !Settings::testCase.compare("Matrix"))
		 runner.addTest(MatrixTests::suite());

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
	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
	std::cout <<"[";
	std::cout << floor(ThermostatDiscrete::currentTemperature * 10000)/10000.0;
	std::cout << " K";
	std::cout <<"]";
	}
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
#ifdef NDEBUG
		std::cout << estSeconds <<"s    \r";
#else
		std::cout << estSeconds <<"s    \n";
#endif
	}
	else{
#ifdef NDEBUG
		std::cout << " " << percentage << "%  Est. Remaining: TBD\r";
#else
		std::cout << " " << percentage << "%  Est. Remaining: TBD\n";
#endif
	}
	std::cout.flush();
}
