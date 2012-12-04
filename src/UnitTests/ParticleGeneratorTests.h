/*
 * ParticleGeneratorTests.h
 *
 *  Created on: 13.11.2012
 *      Author: j
 */

#ifndef PARTICLEGENERATORTESTS_H_
#define PARTICLEGENERATORTESTS_H_

#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>

#include "../utils/Vector.h"
#include "../Particle.h"
#include "../ParticleContainer.h"
#include "../utils/particleGenerator.h"

class ParticleGeneratorTests : public CppUnit::TestFixture {
private:
	ParticleContainer pc;

public:
	void setUp() {

	}

	void tearDown() {

	}

	void testSphereSize(){

		int radius = 6;
		utils::Vector<double, 3> bla;
		bla[0] = 0.0;
		bla[1] = 0.0;
		bla[2] = 0.0;

		ParticleGenerator::generateSphere(pc, bla, radius, 1.1225, 1.0, 1, bla, 0.0);

		CPPUNIT_ASSERT((4.0/3.0)*((radius*radius*radius)-1)<pc.getSize() && pc.getSize()<(4.0/3.0)*((radius*radius*radius)+1));
	}

	void testSphereMaximumDistanceToCenter(){
		int radius = 5;
		utils::Vector<double, 3> bla;
		bla[0] = 0.0;
		bla[1] = 0.0;
		bla[2] = 0.0;

		ParticleGenerator::generateSphere(pc, bla, 5, 1.1225, 1.0, 1, bla, 0.0);
		bool ok = true;
		pc.each([&] (Particle &p){
			ok && (p.x.L2Norm()<radius);
		});
		CPPUNIT_ASSERT(ok);
	}


	/**
	 * create a test suite containing all tests for the ParticleGenerator
	 */
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "ParticleGeneratorTests" );
		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleGeneratorTests>(
				"testSphereSize",
				&ParticleGeneratorTests::testSphereSize) );

		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleGeneratorTests>(
						"testSphereMaximumDistanceToCenter",
						&ParticleGeneratorTests::testSphereMaximumDistanceToCenter) );

		return suiteOfTests;
	}
};


#endif /* PARTICLEGENERATORTESTS_H_ */
