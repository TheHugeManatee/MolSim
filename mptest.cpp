#include <omp.h>
#include <iostream>
#include <stdio.h>


int main(void) {
	#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		printf("Thread %i\n", tid);
	}

	printf("all done!");
}
