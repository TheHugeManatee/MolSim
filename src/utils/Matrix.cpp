/*
 * Matrix.cpp
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#include "Matrix.h"
#include <math.h>
#include <sstream>

namespace utils {

#define PI 3.14159265358979323846

Matrix::Matrix() {
	m[0] = 1; 	m[1] = 0; 	m[2] = 0; 	m[3] = 0;
	m[4] = 0; 	m[5] = 1; 	m[6] = 0; 	m[7] = 0;
	m[8] = 0; 	m[9] = 0; 	m[10] = 1; 	m[11] = 0;
	m[12] = 0; 	m[13] = 0; 	m[14] = 0; 	m[15] = 1;
}

Matrix::Matrix(const double _m[16]) {
	for(int i=0; i < 16; i++)
		m[i] = _m[i];
}

Matrix Matrix::operator*(const Matrix& rhs) {
	double res[16];

	for(int r=0; r < 4; r++) {
		for( int c=0; c < 4; c++) {
			double s = 0;
			for(int k=0; k < 4; k++) {
				s += rhs.m[c + 4*k]*m[k + 4*r];
			}
			res[c + 4*r] = s;
		}
	}

	return Matrix(res);
}


double Matrix::operator[](int i) {
	return m[i];
};
void Matrix::transform(double v[3]) {
	double t[4];
	t[0] = m[ 0]*v[0] + m[ 1]*v[1] + m[ 2]*v[2] + m[ 3];
	t[1] = m[ 4]*v[0] + m[ 5]*v[1] + m[ 6]*v[2] + m[ 7];
	t[2] = m[ 8]*v[0] + m[ 9]*v[1] + m[10]*v[2] + m[11];
	t[3] = m[12]*v[0] + m[13]*v[1] + m[14]*v[2] + m[15];

	v[0] = t[0]/t[3];
	v[1] = t[1]/t[3];
	v[2] = t[2]/t[3];
}

Matrix Matrix::identity() {
	return Matrix();
}

Matrix Matrix::translate(double x0, double x1, double x2) {
	double r[] = {
		1, 0, 0, x0,
		0, 1, 0, x1,
		0, 0, 1, x2,
		0, 0, 0,  1
	};
	return Matrix(r);
}

Matrix Matrix::scale(double a0, double a1, double a2) {
	double r[] = {
		a0, 0, 0, 0,
		0, a1, 0, 0,
		0, 0, a2, 0,
		0, 0, 0,  1
	};
	return Matrix(r);
}

Matrix Matrix::rotatex0(double angle) {
	double th = angle/180*PI;
	double r[] = {
		     1,      0,      0,      0,
		     0,cos(th),-sin(th),     0,
		     0,sin(th), cos(th),     0,
		     0,      0,      0,      1
	};
	return Matrix(r);
}

Matrix Matrix::rotatex1(double angle) {
	double th = angle/180*PI;
	double r[] = {
		cos(th),     0,sin(th),      0,
			 0,      1,      0,      0,
		-sin(th),     0,cos(th),     0,
			 0,      0,      0,      1
	};
	return Matrix(r);
}

Matrix Matrix::rotatex2(double angle) {
	double th = angle/180*PI;
	double r[] = {
	     cos(th),-sin(th),  0, 0,
	     sin(th), cos(th),  0, 0,
	         0,      0,     1, 0,
		     0,      0,     0, 1
	};
	return Matrix(r);
}

Matrix Matrix::shear(double x0x1, double x0x2, double x1x0, double x1x2,
		double x2x0, double x2x1) {
	double r[] = {
		   1, x1x0, x2x0, 0,
		x0x1,    1, x2x1, 0,
		x0x2, x1x2,    1, 0,
		   0,    0,    0, 1
	};
	return Matrix(r);
}

void Matrix::transform(utils::Vector<double, 3> &v) {
	transform(v.content);
}

Matrix Matrix::perspective(double x0, double x1, double x2) {
	double r[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x0, x1, x2,  1
	};
	return Matrix(r);
}

std::string Matrix::toString() {
	std::stringstream ss;

	ss <<  m[0] << "\t" << m[1] << "\t" << m[2] << "\t" << m[3] << "\n" <<
			m[4] << "\t" << m[5] << "\t" << m[6] << "\t" << m[7] << "\n" <<
			m[8] << "\t" << m[9] << "\t" << m[10] << "\t" << m[11] << "\n" <<
			m[12] << "\t" << m[13] << "\t" << m[14] << "\t" << m[15] << "\n";
	return ss.str();
}


utils::Matrix Matrix::buildFromXml(Transform &transformDef) {
	 double x[3] = {0, 0, 0};

	 if(transformDef.x().present()) {
		 x[0] = transformDef.x().get().x0();
		 x[1] = transformDef.x().get().x1();
		 x[2] = transformDef.x().get().x2();
	 }


	 if(transformDef.type() == TransformType::scale)
		 return utils::Matrix::scale(x[0], x[1], x[2]);
	 else if(transformDef.type() == TransformType::translate)
		 return utils::Matrix::translate(x[0], x[1], x[2]);
	 else if(transformDef.type() == TransformType::rotatex0)
		 return utils::Matrix::rotatex0(transformDef.angle());
	 else if(transformDef.type() == TransformType::rotatex1)
		 return utils::Matrix::rotatex1(transformDef.angle());
	 else if(transformDef.type() == TransformType::rotatex2)
		 return utils::Matrix::rotatex2(transformDef.angle());
	 else if(transformDef.type() == TransformType::shear)
		 return utils::Matrix::shear(transformDef.shear01(), transformDef.shear01(), transformDef.shear10(),
				 	 	 	 		transformDef.shear12(), transformDef.shear20(), transformDef.shear21());
	 else if(transformDef.type() == TransformType::perspective)
		 return utils::Matrix::perspective(x[0], x[1], x[2]);

	 return utils::Matrix();

}

} /* namespace utils */
