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

#endif /* SIMULATIONJOBQUEUE_H_ */

#endif
