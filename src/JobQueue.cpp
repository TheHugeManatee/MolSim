/*
 * @file JobQueue.cpp
 *
 * @date 22.01.2013
 * @author Jakob Weiss
 */

#include "JobQueue.h"

#ifdef _OPENMP

#include <omp.h>
#include <stdio.h>

JobQueue::JobQueue() {
	omp_init_lock(&lock_);
}

JobQueue::~JobQueue() {

	for(int i=0; i < jobs.size(); i++)
		delete jobs[i];

 	omp_destroy_lock(&lock_);
}

void JobQueue::enqueue(Job *job) {
	lock();
	queue.push(job);
	unlock();
}

Job * JobQueue::dequeue() {
	lock();

	if(queue.empty()) {
		unlock();
		return NULL;
	}

	Job* job = queue.front();
	queue.pop();

	unlock();
	return job;
};

void JobQueue::executeJobsParallel(CellListContainer *particleContainer, SimulationScenario *scenario) {
#pragma omp parallel
	{
		Job *job;
		while( (job = dequeue()) != NULL ) {
			job->exec(particleContainer, scenario);

			job->enqueueDependentJobs(*this);
		}
	}

}

void JobQueue::resetJobs() {
	int s = jobs.size();
	//this should not run parallel but safety first :)
	lock();
	for(int i=0; i < s; i++) {
		if(jobs[i]->reset())
			queue.push(jobs[i]);
	}
	unlock();
}

void BlockJob::enqueueDependentJobs(JobQueue &queue) {
	{
		if(firstSlice) {
			firstSlice->lock();
			firstSlice->containerDone = true;
			if(firstSlice->predecessorDone) {
				queue.enqueue(firstSlice);
			}
			firstSlice->unlock();
		}
		if(succeedingSlice) {
			succeedingSlice->lock();
			succeedingSlice->predecessorDone = true;
			if(succeedingSlice->containerDone) {
				queue.enqueue(succeedingSlice);
			}
			succeedingSlice->unlock();
		}
	}
}

void BlockJobX0::exec(CellListContainer *container, SimulationScenario *scenario) {

//	printf("##Block from %i to %i\n", start, end);


	int nX0 = container->nX0, nX1 = container->nX1, nX2 = container->nX2;
	std::vector<ParticleContainer> &cells = container->cells;
	double rcSquared = Settings::rCutoff*Settings::rCutoff;

	//for all cells, calculate the force
	for(int x0=start; x0 < end; x0++) {
		for(int x1=1; x1 < nX1-1; x1++) {
			for(int x2=1; x2 < nX2-1; x2++) {
				int cid = x2 + x1*nX2 + x0*nX2*nX1;
				ParticleContainer &c = cells[cid];
				int sc1 = c.getSize();

				//process the appropriate adjacent cells
				//this will be 13 cell comparisons
				//we rely on compiler optimization on this one :)
				if(!sc1) continue;
				c.eachPair(scenario->calculateForce);

				/*(0,1,0)*/	EACHPAIR(scenario->calculateForce, c, cells[cid + nX2]);
				/*(1,0,0)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1]);
				/*(1,1,0)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 + nX2]);
				/*(1,-1,0)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 - nX2]);
				/*(0,0,-1)*/EACHPAIR(scenario->calculateForce, c, cells[cid - 1]);

				/*(0,1,-1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2 - 1]);
				/*(1,0,-1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 - 1]);
				/*(1,1,-1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 + nX2 - 1]);
				/*(1,-1,-1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 - nX2 - 1]);
				/*(0,1,1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2 + 1]);
				/*(1,0,1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 + 1]);
				/*(1,1,1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 + nX2 + 1]);
				/*(1,-1,1)*/EACHPAIR(scenario->calculateForce, c, cells[cid + nX2*nX1 - nX2 + 1]);
			}
		}
	}

	//for all others except the first slice (excluding halo cells), do position, velocity
	for(int x0= start + 1; x0 < end; x0++) {
		for(int x1=2; x1 < nX1-2; x1++) {
			for(int x2=2; x2 < nX2-2; x2++) {
				int cid = x2 + x1*nX2 + x0*nX2*nX1;
				ParticleContainer &c = cells[cid];
				int cs = c.particles.size();

				for(int i=0; i < cs; i++) {
					scenario->addAdditionalForces(c.particles[i]);

					scenario->updatePosition(c.particles[i]);
				}
			}
		}
	}
}



void SliceJobX0::exec(CellListContainer *container, SimulationScenario *scenario) {
	int nX0 = container->nX0, nX1 = container->nX1, nX2 = container->nX2;
	int x0 = sliceIdx;
	std::vector<ParticleContainer> &cells = container->cells;
	double rcSquared = Settings::rCutoff*Settings::rCutoff;

//	printf("||Slice %i\n", sliceIdx);

	for(int x1=2; x1 < nX1-2; x1++) {
		for(int x2=2; x2 < nX2-2; x2++) {
			int cid = x2 + x1*nX2 + sliceIdx*nX2*nX1;
			ParticleContainer &c = cells[cid];
			int cs = c.particles.size();

			for(int i=0; i < cs; i++) {
				scenario->addAdditionalForces(c.particles[i]);

				scenario->updatePosition(c.particles[i]);
			}
		}
	}
}


#endif
