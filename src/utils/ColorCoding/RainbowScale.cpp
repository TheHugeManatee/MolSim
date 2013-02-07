/*
 * RainbowScale.cpp
 *
 *  Created on: 03.02.2013
 *      Author: j
 */

#include "RainbowScale.h"
#include <iostream>

RainbowScale::RainbowScale() {
	huemax = 0;
	huemin = 300;
}

RainbowScale::~RainbowScale() {
	// TODO Auto-generated destructor stub
}

void RainbowScale::getColor(double d, double *c) {
	double t = (d - min)/(maxminusmin);

    double hue = t*(huemax - huemin) + huemin;

	//std::cout << "t: " << t  << " hue: " << hue << " huemax: " << huemax << " huemin: " << huemin <<  std::endl;
    ColorCoder::HSVToRGB(hue, 1.0f, 1.0f, c);
}
