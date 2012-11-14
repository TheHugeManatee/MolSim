/*
 * ParticleContainerTests.h
 *
 *  Created on: 13.11.2012
 *      Author: j
 */

#ifndef PARTICLECONTAINERTESTS_H_
#define PARTICLECONTAINERTESTS_H_

#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>

#include "../utils/Vector.h"
#include "../Particle.h"
#include "../ParticleContainer.h"

class ParticleContainerTests : public CppUnit::TestFixture {
private:
	std::vector<Particle> plist, pListEach;
	ParticleContainer pcAdd, pcEach;
public:
	void setUp() {
		for(int i=0; i < 10; i++) {
			Particle p(i);

			plist.push_back(p);

		}

	}

	void tearDown() {

	}

	/**
	 * tests the following conditions:
	 * - correct size after adding some particles
	 * - every particle added exactly once
	 */
	void testAddEach() {
		pcAdd.add(plist[0]);
		pcAdd.add(plist[1]);
		CPPUNIT_ASSERT_EQUAL(pcAdd.getSize(), 2);

		pcAdd.add(plist[2]);
		CPPUNIT_ASSERT_EQUAL(pcAdd.getSize(), 3);

		int is1Contained = 0;
		int is2Contained = 0;
		int is3Contained = 0;
		pcAdd.each([&] (Particle &p) {
			if(p == plist[0]) is1Contained++;
			if(p == plist[1]) is2Contained++;
			if(p == plist[2]) is3Contained++;
		});

		CPPUNIT_ASSERT(is1Contained == 1);
		CPPUNIT_ASSERT(is2Contained == 1);
		CPPUNIT_ASSERT(is3Contained == 1);
	}

	/**
	 * tests the following conditions:
	 * - correct pairs count (45)
	 * - correct comparison count for every particle (9)
	 * - no particle compared with itself
	 * - no pairs compared twice
	 */
	void testEachPair() {
		for(int i=0; i < 10; i++) {
			pcEach.add(plist[i]);
		}

		int callCount = 0;

		int comparedMatrix[10][10];
		for(int x = 0; x < 10; x++)
			for(int y = 0; y < 10; y++)
				comparedMatrix[x][y] = 0;

		pcEach.eachPair([&] (Particle& p1, Particle& p2) {
			callCount ++;

			p1.getF()[0] = p1.getF()[0] + 1;
			p2.getF()[0] = p2.getF()[0] + 1;

			CPPUNIT_ASSERT(!(p1 == p2));

			CPPUNIT_ASSERT(comparedMatrix[p1.getType()][p2.getType()] == 0);
			CPPUNIT_ASSERT(comparedMatrix[p2.getType()][p1.getType()] == 0);

			comparedMatrix[p1.getType()][p2.getType()] = comparedMatrix[p2.getType()][p1.getType()] = 1;
		});

		pcEach.each([&] (Particle &p) {
			pListEach.push_back(p);
		});

		CPPUNIT_ASSERT(callCount == 45);
		CPPUNIT_ASSERT(pListEach[0].getF()[0] == 9);
	}

	/**
	 * create a test suite containing all tests for the ParticleContainer
	 */
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "ParticleContainerTests" );
		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleContainerTests>(
									   "testAddEach",
									   &ParticleContainerTests::testAddEach ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleContainerTests>(
											   "testEachPair",
											   &ParticleContainerTests::testEachPair ) );
		return suiteOfTests;
	}
};


#endif /* PARTICLECONTAINERTESTS_H_ */
