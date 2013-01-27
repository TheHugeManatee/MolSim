/*
 * SimulationJobQueue.h
 *
 *  Created on: 26.01.2013
 *      Author: j
 */

#ifndef SIMULATIONJOBQUEUE_H_
#define SIMULATIONJOBQUEUE_H_

#include "JobQueue.h"

class SimulationJobQueue: public JobQueue {
public:
	SimulationJobQueue(CellListContainer *cont);
	~SimulationJobQueue();
};

class BlockJob : public Job {
protected:
	const int start, end;
	SliceJob *firstSlice;
	SliceJob *succeedingSlice;

public:
	BlockJob(int start_, int end_, SliceJob *firstSlice_, SliceJob *succeedingSlice_) :
		start(start_), end(end_), firstSlice(firstSlice_), succeedingSlice(succeedingSlice_) {
		std::cout << "Block Job from " << start_ << " to " << end_ << " created." << std::endl;
	};

	void enqueueDependentJobs(JobQueue &queue);
};

class SliceJob : public Job {
protected:
	const int sliceIdx;

public:

	SliceJob(int sliceIdx_) : Job(2), sliceIdx(sliceIdx_) {
		std::cout << "Slice Job " << sliceIdx_ << " created." << std::endl;
	};

	void enqueueDepenedentJobs(JobQueue &queue) {	/*no follow-ups*/};
};


class BlockJobX0 : public BlockJob {
public:
	BlockJobX0(int start_, int end_, SliceJob *firstSlice_, SliceJob *succeedingSlice_) :
		BlockJob(start_, end_, firstSlice_, succeedingSlice_) {	};

	void exec(CellListContainer *container, SimulationScenario *scenario);
};

class SliceJobX0 : public SliceJob {
public:
	SliceJobX0(int sliceIdx_) : SliceJob(sliceIdx_) {};

	void exec(CellListContainer *container, SimulationScenario *scenario);
	void enqueueDependentJobs(JobQueue &queue) {};

};

#endif /* SIMULATIONJOBQUEUE_H_ */
