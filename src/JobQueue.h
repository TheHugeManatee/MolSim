/*
 * @file JobQueue.h
 *
 * @date 22.01.2013
 * @author Jakob Weiss
 */

#ifndef JOBQUEUE_H_
#define JOBQUEUE_H_

#ifdef _OPENMP

#include <omp.h>
#include <queue>

#include "ScenarioFactory.h"
#include "CellListContainer.h"

class Job;
class BlockJob;
class SliceJob;
class JobQueue;


/**
 * @class JobQueue
 *
 * this class manages jobs that can be distributed over different threads
 * the enqueue and dequeue operations are protected by locks to prevent race conditions
 * between threads accessing the queue simultaneously.
 *
 *
 */
class JobQueue {
protected:
	omp_lock_t lock_;
	std::queue<Job *> queue;

	std::vector<Job *> jobs;

public:
	JobQueue();
	~JobQueue();

	void lock() 	{	omp_set_lock(&lock_);	};
	void unlock() 	{	omp_unset_lock(&lock_);	};

	void enqueue(Job *job);
	Job* dequeue();

	void executeJobsParallel(CellListContainer *particleContainer, SimulationScenario *scenario);
	void resetJobs();

};

/**
 * @class Job
 *
 * A basic job object that can be stored in the JobQueue.
 *
 * This can be locked and unlocked with the lock() and unlock() methods respectively for thread safety
 *
 * the operator() can be overwritten to implement the functionality of the job
 *
 * There are two kinds of Jobs that inherit from this class: BlockJob and SliceJob.
 * First the computation domain is divided into several BlockJobs that are always one cell apart. The remaining
 * cells are then pooled into SliceJobs.
 * First BlockJobs are performed in parallel. As soon as two adjacent Blocks are done the intermediary SliceJob
 * can be executed.
 *
 * This rather complicated approach was taken because with blocks the workload can be distributed better across
 * the threads the processor offers, but Blocks cannot be perfectly adjacent to each other because particles
 * at the border region could be calculated by multiple threads at the same time due to Newton's Third Law.
 * Locking each particle wouldn't yield good performance as the overhead is pretty high. But this approach takes
 * these Problems into consideration as first the BlockJobs can be executed in parallel and as soon two neighbouring
 * BlockJobs are done the intermediary SliceJobs can safely be executed.
 *
 */
class Job {
private:
	omp_lock_t lock_;

protected:
	int requiredDependencies;
	int dependenciesFinished;

public:
	Job(int requiredDependencies_ = 0) : requiredDependencies(requiredDependencies_), dependenciesFinished(0)		{
		omp_init_lock(&lock_);
	};
	virtual ~Job() 	{	omp_destroy_lock(&lock_);};

	void lock() 	{	omp_set_lock(&lock_);	};
	void unlock() 	{	omp_unset_lock(&lock_);	};


	virtual void exec(CellListContainer *container, SimulationScenario *scenario) = 0;
	virtual void enqueueDependentJobs(JobQueue &queue) {};

	virtual bool reset() {
		dependenciesFinished = 0;
		return !requiredDependencies;
	};
	virtual void dependencyFinished(JobQueue &queue) {
		lock();
		if(++dependenciesFinished >= requiredDependencies) {
			queue.enqueue(this);
		}
//#pragma omp critical(dbg_io)
//		{std::cout << "Job has " << dependenciesFinished << " out of " << requiredDependencies << std::endl;}
		unlock();

	};
};




#endif /* _OPENMP */

#endif /* JOBQUEUE_H_ */
