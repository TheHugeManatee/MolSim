/*
 * SimulationJobQueue.h
 *
 *  Created on: 26.01.2013
 *      Author: Jakob Weiss
 */

#ifdef _OPENMP

#ifndef SIMULATIONJOBQUEUE_H_
#define SIMULATIONJOBQUEUE_H_

#include "JobQueue.h"

class SimulationJobQueue: public JobQueue {
public:
	SimulationJobQueue(CellListContainer *cont);
	~SimulationJobQueue();
};

class BlockJob : public Job {
	friend class SimulationJobQueue;
protected:
	const int start, end;
	Job *firstSlice;
	Job *succeedingSlice;

public:
	BlockJob(int dependencies, int start_, int end_, Job *firstSlice_, Job *succeedingSlice_) : Job(dependencies),
		start(start_), end(end_), firstSlice(firstSlice_), succeedingSlice(succeedingSlice_)  {
//		std::cout << "Block Job from " << start_ << " to " << end_ << " created; slices are " << firstSlice_ << " and " << succeedingSlice_ <<  std::endl;
	};

	virtual void enqueueDependentJobs(JobQueue &queue);
	void exec(CellListContainer *container, SimulationScenario *scenario);
};

class DependentBlockJob : public BlockJob {
public:
	DependentBlockJob(int dependencies, int start_, int end_, Job *firstSlice_, Job *succeedingSlice_) :
		BlockJob(dependencies, start_, end_, firstSlice_, succeedingSlice_) {
//		std::cout << "  New dependent block job: from " << start_ << " to " << end_ << " dependencies: " << dependencies << std::endl;
	};
};

class BaseBlockJob : public BlockJob {
	friend class SimulationJobQueue;
private:
	Job *prevBlock, *nextBlock;

public:
	BaseBlockJob(int start_, int end_, Job *firstSlice_, Job *succeedingSlice_, Job *prevBlock_ = NULL, Job *nextBlock_ = NULL) :
		BlockJob(0, start_, end_, firstSlice_, succeedingSlice_), prevBlock(prevBlock_), nextBlock(nextBlock_) {
//		std::cout << "  New Base block job from " << start_ << " to " << end_ << "; prev = " << prevBlock_ << "; next = " << nextBlock << std::endl;
	}

	void enqueueDependentJobs(JobQueue &queue);
};

class SliceJob : public Job {
protected:
	const int sliceIdx;

public:

	SliceJob(int sliceIdx_) : Job(2), sliceIdx(sliceIdx_) {
//		std::cout << "Slice Job " << sliceIdx_ << " created." << std::endl;
	};

	void enqueueDepenedentJobs(JobQueue &queue) {	/*no follow-ups*/};
	void exec(CellListContainer *container, SimulationScenario *scenario);
};

#endif /* SIMULATIONJOBQUEUE_H_ */

#endif
