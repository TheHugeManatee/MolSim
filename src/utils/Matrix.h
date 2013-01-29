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

	Matrix operator*(const Matrix &rhs);
	double operator[](int i);

	void transform(double v[3]);
	void transform(utils::Vector<double, 3> &v);
	std::string toString();

	static Matrix identity();
	static Matrix translate(double x0, double x1, double x2);
	static Matrix scale(double a0, double a1, double a2);
	static Matrix rotatex0(double angle);
	static Matrix rotatex1(double angle);
	static Matrix rotatex2(double angle);
	static Matrix shear(double x0x1, double x0x2 = 0, double x1x0 = 0, double x1x2 = 0, double x2x0 = 0, double x2x1 = 0);
	static Matrix perspective(double x0, double x1, double x2);

	/**
	 * Builds a transformation matrix from an xsd sequence of transformations
	 */
	static Matrix buildFromXml(Transform &transformDef);
};

} /* namespace utils */
#endif /* MATRIX_H_ */
