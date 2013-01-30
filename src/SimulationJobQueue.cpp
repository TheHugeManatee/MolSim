/*
 * SimulationJobQueue.cpp
 *
 *  Created on: 26.01.2013
 *      Author: j
 */

#ifdef _OPENMP

#include "SimulationJobQueue.h"

SimulationJobQueue::SimulationJobQueue(CellListContainer *cont) {
	SliceJob *slice = NULL;
	//each job has just one slice.. this seems to be the optimal case..
	int numBlocks = cont->nX0 - 2;

	//enqueue the blocks directly and assign the slices as successors of the blocks
	//note: first slice does not need to be executed because it is a halo slice
	for(int i=0; i < numBlocks; i++) {
		int startSlice = i * (cont->nX0 - 2)/numBlocks + 1;
		int endSlice = (i+1) * (cont->nX0 - 2)/numBlocks + 1;

		SliceJob *nextSlice = (endSlice == cont->nX0)?NULL:(new SliceJobX0(endSlice));
		BlockJob *block = new BlockJobX0(startSlice, endSlice, slice, nextSlice);

		if(nextSlice) jobs.push_back(nextSlice);
		jobs.push_back(block);

		queue.push(block);

		slice = nextSlice;
	}
}

SimulationJobQueue::~SimulationJobQueue() {
	//move along, nothing to see here!
	//destruction of objects happens in base class destructor
}

#endif
