/*
* Simulator.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: j
 */

#include "Simulator.h"

#include "utils/Settings.h"

#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"

#ifndef NOGLVISUALIZER
#include "outputWriter/RenderOutputWriter.h"
#endif

#include "CellListContainer.h"

log4cxx::LoggerPtr Simulator::logger = log4cxx::Logger::getLogger("Simulator");

int Simulator::iterations = 0 ;

Simulator::Simulator() {
	if(Settings::containerType == ContainerType::ParticleContainer) {
		particleContainer = new ParticleContainer();
	}
	else {
		particleContainer = new CellListContainer();

#ifdef _OPENMP
		queue = new JobQueue((CellListContainer*)particleContainer);
#endif
	}

	scenario = ScenarioFactory::build(Settings::scenarioType);

	scenario->setup(*particleContainer);

	particleContainer->afterPositionChanges(scenario->boundaryHandlers);

	/* std::function<bool (ParticleContainer &container, Particle &p)> boundaryHandlers[6];
	for(int i = 0 ; i<6 ; i++){
	boundaryHandlers[i] = [] (ParticleContainer &container, Particle &p) {
		return false;
	};
	}
	particleContainer->afterPositionChanges(boundaryHandlers ,scenario->haloHandler); */
	LOG4CXX_TRACE(logger, "Scenario set up.");
	//pre-calculate the forces for the first update
	particleContainer->eachPair(scenario->calculateForce);
	addAdditionalForces();


	LOG4CXX_INFO(logger, "Simulator set up.");
	LOG4CXX_INFO(logger, "Will run scenario " << Settings::scenarioType);

	int particleContainerSize = particleContainer->getSize();

	LOG4CXX_INFO(logger, "World has " << particleContainerSize << "particles");

	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
		Thermostat::initialize(Settings::dimensions, particleContainerSize);
		Thermostat::scaleInitialVelocity(particleContainer);
	}

	//plotParticles(0);
}

Simulator::~Simulator() {
	delete particleContainer;
	delete scenario;
#ifdef _OPENMP
	delete queue;
#endif
}


inline void Simulator::addAdditionalForces(){

	particleContainer->each(scenario->addAdditionalForces);

}




void Simulator::exportPhaseSpace(void){
	std::ofstream myfile;
	myfile.open (Settings::lastStateFile);
	myfile << "# automatically generated file" << std::endl << "0" << std::endl;
	particleContainer->each([&] (Particle &p){
#ifdef _OPENMP
#pragma omp critical
#endif
		{
			myfile << p.toStringForExport();
			myfile << std::endl;
		}
	});
	myfile.close();
}

void Simulator::printDiffusion(){
	std::ostream myfile;
	myfile.open("Diffusion.csv");
	double diffusion = 0;

	particleContainer->each([&] (Particle &p)){
		diffusion += (p.x - p.x_t0).LengthOptimizedR3Squared();
	}

	int num = particleContainer->getSize();
	diffusion = diffusion / num ;
	myfile << diffusion  << std::endl;
	myfile.close();
}

void Simulator::

void Simulator::plotParticles(int iteration) {
	outputWriter::VTKWriter vtkWriter;
	outputWriter::XYZWriter xyzWriter;


#ifndef NOGLVISUALIZER
	if(Settings::show3DVisual) {
		outputWriter::RenderOutputWriter openglView;
		LOG4CXX_TRACE(logger,"Plotting Particles");
		openglView.plotParticles(*particleContainer, Settings::outputFilePrefix, iteration);
	}
#endif
	if(Settings::disableOutput) return;
	switch (Settings::outputFileType) {
	case OutputFileType::xyz:
		xyzWriter.plotParticles(*particleContainer, Settings::outputFilePrefix, iteration);
	break;
	case OutputFileType::vtk:
		vtkWriter.initializeOutput(particleContainer->getSize());
		particleContainer->each([&] (Particle& p) {
#ifdef _OPENMP
#pragma omp critical (plot_particle)
#endif
			vtkWriter.plotParticle(p);
		});
		vtkWriter.writeFile(Settings::outputFilePrefix, iteration);
	break;
	}
	LOG4CXX_TRACE(logger,"Plotted \t"<< iteration << "\t Particles" );
}


void Simulator::nextTimeStep() {
	static std::function<bool (ParticleContainer &container, Particle &p)> boundaryHandlers[6] = {
			[] (ParticleContainer &container, Particle &p) {return false;},
			[] (ParticleContainer &container, Particle &p) {return false;},
			[] (ParticleContainer &container, Particle &p) {return false;},
			[] (ParticleContainer &container, Particle &p) {return false;},
			[] (ParticleContainer &container, Particle &p) {return false;},
			[] (ParticleContainer &container, Particle &p) {return false;}
	};


#ifdef _OPENMP
#pragma omp parallel
	{
		Job *job;
		while( (job = queue->dequeue()) != NULL ) {
			(*job)((CellListContainer *)particleContainer, scenario);

			job->enqueueDependentJobs(*queue);
		}
	}

	queue->resetJobs();
#else
	//Calculate all forces
	particleContainer->eachPair(scenario->calculateForce);
	particleContainer->each(scenario->addAdditionalForces);

	//update velocity and position
	particleContainer->each(scenario->updatePosition);
#endif

	particleContainer->clearHalo();


	if(Settings::thermostatSwitch == SimulationConfig::ThermostatSwitchType::ON){
		ThermostatDiscrete::updateThermostate(particleContainer);
	}


	//rearrange internal particle container structure											/*This move is ugly but necessary for periodic boundary Handling*/
	particleContainer->afterPositionChanges(scenario->boundaryHandlers);	/*Without it we wouldn't have any way to calculate forces between */
																					/*two opposite cells*/
																								/*TODO:Maybe we should separate afterPostionChanges in one method
																								*applying bounderyHandling an one to apply haloHandling and sort the cells
																								*after force calculation*/


	Simulator::iterations++;
	LOG4CXX_TRACE(logger,"Iteration number " << Simulator::iterations); //This one is pretty annoying
}



