/*
 * APCJobQueue.cpp
 *
 *  Created on: 26.01.2013
 *      Author: j
 */

#ifdef _OPENMP

#include "APCJobQueue.h"

#include <vector>

APCJobQueue::APCJobQueue(CellListContainer *cont) {

	int n = cont->nX0 - 2;

	std::vector<APCSliceJob *> tmpList;

	tmpList.reserve(n);

	APCSliceJob *curJob;


	//first the dependent slice jobs
	for(int i=0; i < n; i+=2) {
		if(n%2 && !i)
			curJob = new APCFirstSliceJob(i+1);
		else
			curJob = new APCSliceJob(i+1, false, false);

		jobs.push_back(curJob);
		tmpList.push_back(curJob);
	}

	//if we have an odd number of slices, we need to link the first and last slices together
	if(n%2) {
		((APCFirstSliceJob*)tmpList[0])->lastJob = tmpList.back();

		//std::cout << "LastSlice instance initialized: sliceIdx " << tmpList.back() << ", firstJob reference = " << tmpList.back()->sliceIdx << " (" << tmpList.back() << std::endl;
	}


	int nDependentSlices = tmpList.size();

	for(int i=1; i < n; i+=2) {
		curJob = new APCSliceBaseJob(i+1, tmpList[i/2], tmpList[(i/2+1)%nDependentSlices]);

		queue.push(curJob);
		jobs.push_back(curJob);
	}


}

APCJobQueue::~APCJobQueue() {
	// TODO Auto-generated destructor stub
}

void APCSliceJob::exec(CellListContainer *container, SimulationScenario *scenario) {
	int x0 = sliceIdx;
	int nX0 = container->nX0, nX1 = container->nX1, nX2 = container->nX2;


#pragma omp critical(dbg_io)
	{		std::cout << "APC for slice no. " << x0 << std::endl;	}

	for(int x1=2; x1 < nX1-2; x1++) {
		for(int x2=2; x2 < nX2-2; x2++) {
			ParticleContainer &c = container->cells[x2 + x1*nX2 + x0*nX2*nX1];
			int cellParticleCount = c.particles.size();
			for(int i = 0; i < cellParticleCount; i++) {
				Particle &p = c.particles[i];


				bool particleToBeRemoved = false;

				//Check for all boundaries
				if(x0 == 2) 			particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[0](*container, p);
				if(x0 == (nX0 - 3)) 	particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[1](*container, p);
				if(x1 == 2) 			particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[2](*container, p);
				if(x1 == (nX1 - 3)) 	particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[3](*container, p);
				if(x2 == 2) 			particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[4](*container, p);
				if(x2 == (nX2 - 3)) 	particleToBeRemoved = particleToBeRemoved || scenario->boundaryHandlers[5](*container, p);

				//if particle is not deleted, check if it should be in some different cell than it is now
				if(!particleToBeRemoved) {
					ParticleContainer *cc = container->getContainingCell(p);
					if(cc != &c) {
						cc->add(p);
						particleToBeRemoved = true;
					}
				}
				//Perform the removal
				if(particleToBeRemoved) {
					//switch last one and the one to be delete
					c.particles[i] = c.particles[c.particles.size() - 1];
					//then delete the new last one and decrement the loop counter to check
					//the one at position i again
					c.particles.pop_back();
					cellParticleCount--;
					i--;
				}
			}
		} //for x2
	} //for x1

}

#endif
