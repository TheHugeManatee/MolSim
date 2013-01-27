/*
 * MatrixTests.h
 *
 *  Created on: 26.01.2013
 *      Author: j
 */

#ifndef MATRIXTESTS_H_
#define MATRIXTESTS_H_

#include <vector>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include "utils/Matrix.h"

class MatrixTests : public CppUnit::TestFixture {
private:

public:
	void setUp() {

	}

	void tearDown() {

	}


	void testBuilder () {
		utils::Matrix scale = utils::Matrix::scale(1,2,3),
				rot = utils::Matrix::rotatex0(45),
				trans = utils::Matrix::translate(10, 11, 12),
				shear = utils::Matrix::shear(1, 2 , 10, 12, 20, 21);

		CPPUNIT_ASSERT(scale.m[0] == 1 && scale.m[5] == 2 && scale.m[10] == 3);
		double k1 = cos(45.0/180.0*M_PI),
			   k2 = sin(45.0/180.0*M_PI);

		CPPUNIT_ASSERT(rot[0] == 1 && rot[5] == k1 && rot[6] == -k2 && rot[9] == k2 && rot[10] == k1);
		CPPUNIT_ASSERT(trans[0] == 1 && trans[1] == 0 && trans[3] == 10 && trans[7] == 11 && trans[11] == 12 && trans[15] == 1);
		CPPUNIT_ASSERT(shear[1] == 10 && shear[2] == 20 && shear[4] == 1 && shear[6] == 21 && shear[8] == 2 && shear[9] == 12);
	}

	void testMatrixMult () {
		double a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
		double b[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

		//wolfram alpha says: a*b = 2*r
		double r[] = {
				50, 55, 60, 65,
				114, 127, 140, 153,
				178, 199, 220, 241,
				242, 271, 300, 329
		};

		utils::Matrix c = (utils::Matrix(a)*utils::Matrix(b));

		for(int i=0; i < 16; i++) CPPUNIT_ASSERT(c[i] == 2*r[i]);
	}

	void testMatrixVectorMult() {
		double r[] = {
				50, 55, 60, 65,
				114, 127, 140, 153,
				178, 199, 220, 241,
				0, 0, 0, 1
		};
		double v[] = {1,2,3};

		utils::Matrix(r).transform(v);

		CPPUNIT_ASSERT(v[0] = 405 && v[1] == 941 && v[2] == 1477);
	}

	/**
	 * create a test suite containing all tests for the ParticleGenerator
	 */
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "MatrixTests" );
				suiteOfTests->addTest( new CppUnit::TestCaller<MatrixTests>(
						"testBuilder",
						&MatrixTests::testBuilder) );

		suiteOfTests->addTest( new CppUnit::TestCaller<MatrixTests>(
				"testMatrixVectorMult",
				&MatrixTests::testMatrixVectorMult) );

		suiteOfTests->addTest( new CppUnit::TestCaller<MatrixTests>(
						"testMatrixMult",
						&MatrixTests::testMatrixMult) );

		return suiteOfTests;
	}
};



#endif /* MATRIXTESTS_H_ */
