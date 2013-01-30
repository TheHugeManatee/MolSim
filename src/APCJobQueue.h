/*
 * APCJobQueue.h
 *
 *  Created on: 26.01.2013
 *      Author: Jakob Weiss
 */

#ifdef _OPENMP

#ifndef APCJOBQUEUE_H_
#define APCJOBQUEUE_H_

#include "JobQueue.h"

class APCSliceJob;
class APCSliceBaseJob;

/**
 * @class APCJobQueue
 *
 * As the afterPositionChanges can also be parallelized but have to be calculated after all
 * other Jobs are completed there is an extra queue for them.
 *
 * The idea is basically the same as the "normal" JobQueue, but some extra precautions have to be taken as
 * some of these jobs may have different prerequisites than others.
 *
 *
 */
class APCJobQueue: public JobQueue {
public:
	APCJobQueue(CellListContainer *cont);
	~APCJobQueue();
};


class APCSliceJob : public Job {
	friend class APCJobQueue;

public:
	const int sliceIdx;


public:
	bool prevDone;
	bool nextDone;
	APCSliceJob(int sliceIdx_, bool prevDone_, bool nextDone_) : sliceIdx(sliceIdx_), prevDone(prevDone_), nextDone(nextDone_) {
//		std::cout << "  General job instace: slice " << sliceIdx_ << std::endl;
	};

	void exec(CellListContainer *container, SimulationScenario *scenario);

	bool reset() {
		prevDone = false;
		nextDone = false;
		return false;
	}
};

class APCFirstSliceJob : public APCSliceJob {
	friend class APCJobQueue;

private:
	APCSliceJob *lastJob;

public:
	APCFirstSliceJob(int sliceIdx_) : APCSliceJob(sliceIdx_, true, false), lastJob(NULL) {
	};

	void enqueueDependentJobs(JobQueue &queue) {
		if(lastJob) {
			#pragma omp critical(dbg_io)
		{			std::cout << "LastSlice " << sliceIdx << " is trying to enqueue " << lastJob->sliceIdx << std::endl;		}
			lastJob->lock();
			lastJob->nextDone = true;
			if(lastJob->prevDone) {
#pragma omp critical(dbg_io)
				{			std::cout << "LastSlice  " << sliceIdx << "is enqueueing " << lastJob->sliceIdx << std::endl;		}
				queue.enqueue(lastJob);
			}
			lastJob->unlock();
		}
	};
	bool reset() {
		prevDone = true;
		nextDone = false;
		return false;
	}
};

class APCSliceBaseJob : public APCSliceJob {
	friend class APCJobQueue;

protected:
	APCSliceJob *prevJob, *nextJob;

public:
	APCSliceBaseJob(int sliceIdx_, APCSliceJob * prevJob_, APCSliceJob * nextJob_) : APCSliceJob(sliceIdx_, true, true), prevJob(prevJob_), nextJob(nextJob_) {
//		std::cout << "Base job instace: slice " << sliceIdx_ << ", prev Container = " << prevJob_ << ", nextContainer = " << nextJob_ << std::endl;
	};

	//these jobs have no prerequisites
	bool reset() {
		prevDone = true;
		nextDone = true;
		return true;
	};

	void enqueueDependentJobs(JobQueue &queue) {
#pragma omp critical(dbg_io)
		{			std::cout << "slice " << sliceIdx << " is trying to enqueue " << prevJob->sliceIdx << " and " << nextJob->sliceIdx << std::endl;		}
		if(prevJob) {
			prevJob->lock();
			prevJob->nextDone = true;
			if(prevJob->prevDone) {
#pragma omp critical(dbg_io)
				{			std::cout << "slice " << sliceIdx << " is enqueueing " << prevJob->sliceIdx << std::endl;		}
				queue.enqueue(prevJob);
			}
			prevJob->unlock();
		}
		if(nextJob) {
			nextJob->lock();
			nextJob->prevDone = true;
			if(nextJob->nextDone) {
#pragma omp critical(dbg_io)
				{			std::cout << "slice " << sliceIdx << " is enqueueing " << nextJob->sliceIdx << std::endl;		}
				queue.enqueue(nextJob);
			}
			nextJob->unlock();
		}
	}
};

#endif /* APCJOBQUEUE_H_ */

#endif
