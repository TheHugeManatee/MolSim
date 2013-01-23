/*
 * JobQueue.h
 *
 *  Created on: 22.01.2013
 *      Author: j
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

class JobQueue {
private:
	omp_lock_t lock_;
	std::queue<Job *> queue;

	std::vector<Job *> jobs;

public:
	JobQueue(CellListContainer *cont);
	~JobQueue();

	void lock() 	{	omp_set_lock(&lock_);	};
	void unlock() 	{	omp_unset_lock(&lock_);	};

	void enqueue(Job *job);
	Job* dequeue();

	void resetJobs();

};

class Job {
private:
	omp_lock_t lock_;

public:
	Job() 			{	omp_init_lock(&lock_);	};
	virtual ~Job() 	{	omp_destroy_lock(&lock_);};

	void lock() 	{	omp_set_lock(&lock_);	};
	void unlock() 	{	omp_unset_lock(&lock_);	};


	virtual void operator() (CellListContainer *container, SimulationScenario *scenario) = 0;
	virtual void enqueueDependentJobs(JobQueue &queue) {};
	virtual bool reset() { return true; };
};


class BlockJob : public Job {
protected:
	const int start, end;
	SliceJob *firstSlice;
	SliceJob *succeedingSlice;

public:
	BlockJob(int start_, int end_, SliceJob *firstSlice_, SliceJob *succeedingSlice_) :
		start(start_), end(end_), firstSlice(firstSlice_), succeedingSlice(succeedingSlice_) {};

	void enqueueDependentJobs(JobQueue &queue);
};

class SliceJob : public Job {
protected:
	const int sliceIdx;

public:
	bool containerDone;
	bool predecessorDone;
	SliceJob(int sliceIdx_) : sliceIdx(sliceIdx_), containerDone(false), predecessorDone(false) {};

	void enqueueDepenedentJobs(JobQueue &queue) {	/*no follow-ups*/};
	bool reset() {
		containerDone = false;
		predecessorDone = false;
		return false;
	};
};


class BlockJobX0 : public BlockJob {
public:
	BlockJobX0(int start_, int end_, SliceJob *firstSlice_, SliceJob *succeedingSlice_) :
		BlockJob(start_, end_, firstSlice_, succeedingSlice_) {	};

	void operator() (CellListContainer *container, SimulationScenario *scenario);
};

class SliceJobX0 : public SliceJob {
public:
	SliceJobX0(int sliceIdx_) : SliceJob(sliceIdx_) {};

	void operator() (CellListContainer *container, SimulationScenario *scenario);
	void enqueueDependentJobs(JobQueue &queue) {};

};

#endif /* _OPENMP */

#endif /* JOBQUEUE_H_ */
