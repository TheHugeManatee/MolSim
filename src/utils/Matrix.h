/*
 * Matrix.h
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>
#include "Vector.h"

namespace utils {

class Matrix {
public:
	double m[16];

	Matrix();
	Matrix(const double _m[16]);

	Matrix operator*(const Matrix &rhs);

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
};

} /* namespace utils */
#endif /* MATRIX_H_ */
