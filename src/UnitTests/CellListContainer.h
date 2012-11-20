/*
 * CellListContainer.h
 *
 *  Created on: 20.11.2012
 *      Author: Leo
 */

#ifndef CELLLISTCONTAINER_H_
#define CELLLISTCONTAINER_H_

#include <cppunit/TestFixture.h>

class CellListContainer : public CppUnit::TestFixture{




	/**
	 * create a test suite containing all tests for the ParticleContainer
	 */
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "CellListContainer" );
		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleContainerTests>(
									   "testAddEach",
									   &ParticleContainerTests::testAddEach ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<ParticleContainerTests>(
											   "testEachPair",
											   &ParticleContainerTests::testEachPair ) );
		return suiteOfTests;
	}

};

#endif /* CELLLISTCONTAINER_H_ */
