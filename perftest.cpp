#include <vector>
#include <iostream>
#include <sys/timeb.h>
#include <cstdlib>
#include <algorithm>

#define PUT_BLOCKS(A, B, C, D) \
	for(int k=0; k<r; k++) {    \
    A;                          \
    B;                        \
    C;                        \
    D;                          \
    std::cout << "." << std::flush; \
	}                               \
	std::cout << std::endl;       \
	
	
#define ITERATOR_BLOCK \
		startTime = getMilliCount();                        \
		for(auto it = l.begin(); it != l.end(); ++it) {    \
			(*it).f = (*it).a * (*it).a + (*it).d * (*it).e;  \
		}                                                   \
		endTime = getMilliCount();                          \
		timeIterator += (endTime - startTime);              \
		std::cout << "." << std::flush;                     

#define LAMBDA_BLOCK \
		startTime = getMilliCount();                        \
		std::for_each(l.begin(), l.end(), [] (test &tt) {   \
			tt.f = tt.a * tt.a + tt.d * tt.e;                 \
		});                                                 \
		endTime = getMilliCount();                          \
		timeLambda += (endTime - startTime);                
		
#define FOR_BLOCK \
		startTime = getMilliCount();                        \
		for(int i=0; i < n; i++) {                         \
			l[i].f = l[i].a * l[i].a + l[i].d * l[i].e;       \
		}                                                   \
		endTime = getMilliCount();                          \
		timeLoop += (endTime - startTime);                

#define OPT_FOR_BLOCK \
		startTime = getMilliCount();                        \
		for(int i=0; i < l.size(); i++) {                  \
			l[i].f = l[i].a * l[i].a + l[i].d * l[i].e;       \
		}                                                   \
		endTime = getMilliCount();                          \
		timeExtFor += (endTime - startTime);
	
		
#define EXT_FOR_BLOCK \
		startTime = getMilliCount();                        \
		for(test &tt : l) {                                 \
			tt.f = tt.a * tt.a + tt.d * tt.e;                 \
		}                                                   \
		endTime = getMilliCount();                          \
		timeExtFor += (endTime - startTime);
		
typedef struct {
	int a,b,c;
	double d, e, f;
} test;

int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;	
	return nCount;
}

int main() {
	std::vector<test> l;

	int n = 20000000;
	int r = 5;
	std::cout << "Setting up..." << std::endl;

	test t;
	t.a = 2;
	t.b = 12;
	t.c = 123;
	t.d = 12.012;
	t.e = 123.2123;
	t.f = 0.00001;

	l.resize(n);
	//init
	for(int i=0; i < n; i++) {
		t.a = i;
		t.b = 2*i + 1;
		t.d = i/2.0 + i*i;
		l.push_back(t);
	}


	int startTime, endTime;
	int timeLambda = 0;
	int timeIterator = 0;
	int timeLoop = 0;
	int timeExtFor = 0;

  std::cout << "Working..." << std::endl;

  PUT_BLOCKS(ITERATOR_BLOCK, LAMBDA_BLOCK, FOR_BLOCK, OPT_FOR_BLOCK);

	std::cout << "lambda          " << timeLambda << std::endl;
	std::cout << "for loop:       " << timeLoop << std::endl;
	std::cout << "extended for:   " << timeExtFor << std::endl;
	std::cout << "iterators:      " << timeIterator << std::endl;

	return 0;
}
