/*
 * ColorCoder.cpp
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#include "ColorCoder.h"

ColorCoder::ColorCoder(): min(0), max(1) {


}

ColorCoder::~ColorCoder() {
	// TODO Auto-generated destructor stub
}

void ColorCoder::getColor(double d, double *c) {
	c[0] = c[1] = c[2] = 0.0;
}
