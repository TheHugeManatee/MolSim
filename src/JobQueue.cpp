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



#endif
