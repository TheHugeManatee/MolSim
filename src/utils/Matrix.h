/*
 * Matrix.h
 *
 *  Created on: 17.01.2013
 *      Author: Jokob Weiss
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>
#include "Vector.h"
#include "SimulationConfig.h"

namespace utils {


/**
 * @class Matrix
 *
 * This class implements Matrices in order to give the user the ability to translate, scale, rotate by all dimensions etc
 * all objects that are supported by the ParticleGenerator
 *
 *
 */
class Matrix {
public:
	double m[16];

	Matrix();
	Matrix(const double _m[16]);

	/**
	 * returns the Matrix product of the matrix and the passed Matrix
	 * @param rhs Matrix to be multiplied with
	 */
	Matrix operator*(const Matrix &rhs);

	/**
	 * returns entry in the matrix at the index
	 * @param i index
	 */
	double operator[](int i);

	/**
	 * multiplies given vector with the Matrix with dehomogenization
	 * @param v vector to be multipied
	 */
	void transform(double v[3]);

	/**
	 * multiplies given vector with the Matrix with dehomogenization
	 * @param v vector to be multipied
	 */
	void transform(utils::Vector<double, 3> &v);
	std::string toString();

	/**
	 * returns a Matrix with zeroes in all places except for the diagonal which contains all ones (aka Identity)
	 */
	static Matrix identity();

	/**
	 * returns a Matrix to translate particle generation along all three axes
	 * @param x0 Translation along x0 axis
	 * @param x1 Translation along x1 axis
	 * @param x2 Translation along x2 axis
	 */
	static Matrix translate(double x0, double x1, double x2);

	/**
	 * returns a Matrix to scale particle generation along all three axes
	 * @param a0 Translation along x0 axis
	 * @param a1 Translation along x1 axis
	 * @param a2 Translation along x2 axis
	 */
	static Matrix scale(double a0, double a1, double a2);

	/**
	 * returns a Matrix to rotate particle generation along x0
	 * @param angle The angle by which the generation is to be rotated
	 */
	static Matrix rotatex0(double angle);

	/**
	 * returns a Matrix to rotate particle generation along x1
	 * @param angle The angle by which the generation is to be rotated
	 */
	static Matrix rotatex1(double angle);

	/**
	 * returns a Matrix to rotate particle generation along x2
	 * @param angle The angle by which the generation is to be rotated
	 */
	static Matrix rotatex2(double angle);

	/**
	 * returns a Matrix to shear particle generation
	 *
	 * refer to something like http://web.iitd.ac.in/~hegde/cad/lecture/L6_3dtrans.pdf
	 * in order to understand how to use it ;)
	 *
	 */
	static Matrix shear(double x0x1, double x0x2 = 0, double x1x0 = 0, double x1x2 = 0, double x2x0 = 0, double x2x1 = 0);

	/**
	 * returns a Matrix to project particle generation
	 *
	 * refer to something like http://en.wikipedia.org/wiki/Transformation_matrix#Perspective_projection
	 * in order to understand how to use it ;)
	 *
	 */
	static Matrix perspective(double x0, double x1, double x2);

	/**
	 * Builds a transformation matrix from an xsd sequence of transformations
	 */
	static Matrix buildFromXml(Transform &transformDef);
};

} /* namespace utils */
#endif /* MATRIX_H_ */
