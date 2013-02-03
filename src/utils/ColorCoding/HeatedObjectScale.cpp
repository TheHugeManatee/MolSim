/*
 * HeatedObject.cpp
 *
 *  Created on: 03.02.2013
 *      Author: j
 */

#include "HeatedObjectScale.h"

HeatedObjectScale::HeatedObjectScale() {
	// TODO Auto-generated constructor stub

}

HeatedObjectScale::~HeatedObjectScale() {
	// TODO Auto-generated destructor stub
}

void HeatedObjectScale::getColor(double d, double* c) {
	double val = (d - min)/maxminusmin;

	if(val <= 1.0f/3.0f) {
		c[0] = 3.0f*val;
		c[1] = 0.0f;
		c[2] = 0.0f;
	}
	else if(val > 1.0f/3.0f && val <= 2.0f/3.0f) {
		c[0] = 1.0f;
		c[1] = 3.0f*val - 1.0f;
		c[2] = 0.0f;
	}
	else if(val > 2.0f/3.0f) {
		c[0] = 1.0f;
		c[1] = 1.0f;
		c[2] = 3.0f*val - 2.0f;
	}
}
