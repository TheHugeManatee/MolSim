/*
 * CellListContainerTest.h
 *
 *  Created on: 20.11.2012
 *      Author: Leo
 */

#ifndef CELLLISTCONTAINERTEST_H_
#define CELLLISTCONTAINERTEST_H_

#include <vector>
#include <cppunit/TestFixture.h>
#include <random>
#include "utils/Settings.h"
#include "utils/Vector.h"
#include "Particle.h"
#include "CellListContainer.h"



class CellListContainerTest : public CppUnit::TestFixture{

private:
	std::vector<Particle> testArray ;
	CellListContainer testContainer;

public:
	void setUp(){
		auto h = Settings::rCutoff;
		for( int i = 0 ; i<10 ; i++){
			for(int j = 0 ; j<10 ; j++){
				for(int k = 0 ; k<10 ;k++){
					utils::Vector<double, 3> x;
					x[0] = i + 	rand() / RAND_MAX  * h;
					x[1] = j +  rand() / RAND_MAX  * h;
					x[2] = k +  rand() / RAND_MAX  * h;
					utils::Vector<double, 3> y;
					y[0]=0;
					y[1]=0;
					y[2]=0;
					Particle p(x,y,0,);
					testArray.push_back(p);
				}
			}
		}
	}

	void testAdd(){
		int nX0=testContainer.nX0;
		int nX1=testContainer.nX1;
		int nX2=testContainer.nX2;
		for( int i = 0 ; i < testArray.size(); i++){
			testContainer.add(testArray[i]);
		}
			for( int i = 0 ; i<10 ; i++){
				for(int j = 0 ; i<10 ; i++){
					for(int k = 0 ; i<10 ;i++){

						CPPUNIT_ASSERT(testContainer.cells[i+j*nX1+k*nX1*nX2].particles[0]
						                                      ==testArray[i+j*10+k*100]);
					}
				}
			}
	}

	void testEachUpdateCells(){
		int nX0=testContainer.nX0;
		int nX1=testContainer.nX1;
		int nX2=testContainer.nX2;
		auto h = Settings::rCutoff;
		testContainer.each([&testContainer] (Particle p){
			p.x=(utils::Vector<double, 3>(nX0*h,nX1*h,nX2*h) - p.x);
		} );

		testContainer.updateCells();

		for( int i = 0 ; i<10 ; i++){
			for(int j = 0 ; i<10 ; i++){
				for(int k = 0 ; i<10 ;i++){
					CPPUNIT_ASSERT(testContainer.cells[i+j*nX1+k*nX1*nX2].particles[0]==testArray[(10-i)+(10-j)*10+(10-k)*100]);
				}
			}
		}
	}


	void testGetCell(){
		int xr = 5;
		int yr = 5;
		int zr = 5;
		CellListContainer container = testContainer.getCell(xr,yr,zr);
			container.each([](Particle p){
			CPPUNIT_ASSERT(p.x[0]>(xr+1)*h);
			CPPUNIT_ASSERT(p.x[1]>(yr+1)*h);
			CPPUNIT_ASSERT(p.x[2]>(zr+1)*h);
			CPPUNIT_ASSERT(p.x[0]<(xr)*h);
			CPPUNIT_ASSERT(p.x[1]<(yr)*h);
			CPPUNIT_ASSERT(p.x[2]<(zr)*h);
			});
	}

	void testEachPair(){

	}



	/**
	 * create a test suite containing all tests for the ParticleContainer
	 */
	static CppUnit::Test *suite(){
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "CellListContainer" );
		suiteOfTests->addTest( new CppUnit::TestCaller<CellListContainerTest>("testAdd",&CellListContainerTest::testAdd ));
		suiteOfTests->addTest( new CppUnit::TestCaller<CellListContainerTest>("testAdd",&CellListContainerTest::testEachUpdateCells ));
		suiteOfTests->addTest( new CppUnit::TestCaller<CellListContainerTest>("testAdd",&CellListContainerTest::testGetCell ));
		return suiteOfTests;
	}

};

#endif /* CELLLISTCONTAINER_H_ */
