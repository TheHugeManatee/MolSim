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
#include "../utils/ParticleGenerator.h"

#define CIRCLECONSTANT 3.1415926535

class ParticleGeneratorTests : public CppUnit::TestFixture {
private:
	ParticleContainer pcSphereSize, pcSphereDistance, pcCuboid;

public:
	void setUp() {

	}

	void tearDown() {

	}


	/**
	 * tests the following conditions:
	 * - correct amount of particles for a generated cuboid
	 * - whether all particles are within expected area
	 */
	void testCuboid(){
		int length = 3;
		int width = 4;
		int height = 5;

		utils::Vector<double, 3> zero;
		zero[0] = 0.0;
		zero[1] = 0.0;
		zero[2] = 0.0;

		ParticleGenerator::regularCuboid(pcCuboid, zero, length, width, height,1,1,zero,0.1);

		CPPUNIT_ASSERT(pcCuboid.getSize() == length*width*height);

		bool ok = true;
		pcCuboid.each([&] (Particle &p){
			//std::cout << "length: " <<p.x[0] << ", width: " << p.x[1] << ", height: " << p.x[2] <<std::endl;
			ok = ok && (p.x[0]<=length && p.x[1] <= width && p.x[2] <= height);
				});
		CPPUNIT_ASSERT(ok);
	}
	/**
	 * tests the following conditions:
	 * - correct amount of particles for a generated sphere (approximated by volume of smaller and larger sphere)
	 */
	void testSphereSize(){

		int radius = 10;
		utils::Vector<double, 3> zero;
		zero[0] = 0.0;
		zero[1] = 0.0;
		zero[2] = 0.0;

		ParticleGenerator::generateSphere(pcSphereSize, zero, radius, 1.1225, 1, zero, 0.0);
		//std::cout << "size:  " << pcSize.getSize() << "Particles" << std::endl;
		int rmo = radius - 1; //radius minus one
		int rpo = radius + 1; //radius plus one
		CPPUNIT_ASSERT(CIRCLECONSTANT*(4.0/3.0)*((rmo*rmo*rmo))<pcSphereSize.getSize() && pcSphereSize.getSize() < CIRCLECONSTANT*(4.0/3.0)*((rpo*rpo*rpo)));
	}


	/**
		 * tests the following conditions:
		 * - whether all particles are within expected area
		 */
	void testSphereMaximumDistanceToCenter(){
		int radius = 10;
		utils::Vector<double, 3> zero;
		zero[0] = 0.0;
		zero[1] = 0.0;
		zero[2] = 0.0;
		double distance = 1.1225;

		ParticleGenerator::generateSphere(pcSphereDistance, zero, radius, distance, 1, zero, 0.0);
		bool ok = true;
		pcSphereDistance.each([&] (Particle &p){
			ok = ok && (p.x.L2Norm()<radius*distance);
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
						"testCuboid",
						&ParticleGeneratorTests::testCuboid) );

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
