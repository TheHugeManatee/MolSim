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
	int numBlocks = cont->nX0 - 3;

	std::vector<BlockJob *> blockList;

	//enqueue the blocks directly and assign the slices as successors of the blocks
	//note: first slice does not need to be executed because it is a halo slice
	for(int i=0; i < numBlocks; i++) {
		int startSlice = i * (cont->nX0 - 3)/numBlocks + 1;
		int endSlice = (i+1) * (cont->nX0 - 3)/numBlocks + 1;

		SliceJob *nextSlice = (endSlice == cont->nX0-2)?NULL:(new SliceJob(endSlice));
		BlockJob *block;

		//odd ones will be base jobs, even ones will depend on the adjacent odd ones
		if(i%2) {
			block = new BaseBlockJob(startSlice, endSlice, slice, nextSlice);
			queue.push(block);
		}
		else
			block = new DependentBlockJob((!i||i==(numBlocks-1))?1:2, startSlice, endSlice, slice, nextSlice);

		if(nextSlice) jobs.push_back(nextSlice);
		jobs.push_back(block);

		blockList.push_back(block);

		slice = nextSlice;
	}

	//loop over all blocks to tie the dependent ones to the independent
	int bCount = blockList.size();
	for(int i=1; i < bCount; i+=2) {
		((BaseBlockJob *)blockList[i])->prevBlock = blockList[i-1];
		((BaseBlockJob *)blockList[i])->nextBlock = (i<(bCount - 1))?blockList[i+1]:NULL;
		std::cout << "Block from " << blockList[i]->start << " to " << blockList[i]->end << " has prev=" << blockList[i-1] << " and next = " << ( (i<(bCount - 1))?blockList[i+1]:NULL )<< std::endl;
	}
}

SimulationJobQueue::~SimulationJobQueue() {
	//move along, nothing happening here!
	//destruction of objects happens in base class destructor
}

void BlockJob::enqueueDependentJobs(JobQueue &queue) {

	if(firstSlice) {
//#pragma omp critical(dbg_io)
//	{std::cout  << "Block job from " << start << " to " << end << " enqueueing firstslice..." << std::endl; }
		firstSlice->dependencyFinished(queue);
	}
	if(succeedingSlice) {
//#pragma omp critical(dbg_io)
//		{std::cout  << "Block job from " << start << " to " << end << " enqueueing nextSlice..." << std::endl; }
		succeedingSlice->dependencyFinished(queue);
	}
}

void BlockJob::exec(CellListContainer *container, SimulationScenario *scenario) {

//#pragma omp critical(dbg_io)
//	{std::cout  << "Block job from " << start << " to " << end << " executing..." << std::endl; }

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



void SliceJob::exec(CellListContainer *container, SimulationScenario *scenario) {
	int nX0 = container->nX0, nX1 = container->nX1, nX2 = container->nX2;
	int x0 = sliceIdx;
	std::vector<ParticleContainer> &cells = container->cells;
	double rcSquared = Settings::rCutoff*Settings::rCutoff;


//#pragma omp critical(dbg_io)
//	{std::cout  << "slice " << sliceIdx << " executing..." << std::endl; }

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

void BaseBlockJob::enqueueDependentJobs(JobQueue &queue) {

	if(firstSlice) {
//#pragma omp critical(dbg_io)
//	{std::cout  << "Block job from " << start << " to " << end << " enqueueing firstslice..." << std::endl; }
		firstSlice->dependencyFinished(queue);
	}
	if(succeedingSlice) {
//#pragma omp critical(dbg_io)
//		{std::cout  << "Block job from " << start << " to " << end << " enqueueing nextSlice..." << std::endl; }
		succeedingSlice->dependencyFinished(queue);
	}

	if(prevBlock) {
//		#pragma omp critical(dbg_io)
//				{std::cout  << "Block job from " << start << " to " << end << " enqueueing prevBlock..." << std::endl;}
		prevBlock->dependencyFinished(queue);
	}
	if(nextBlock) {
//		#pragma omp critical(dbg_io)
//				{std::cout  << "Block job from " << start << " to " << end << " enqueueing nextBlock..." << std::endl;}
		nextBlock->dependencyFinished(queue);
	}

}

#endif

