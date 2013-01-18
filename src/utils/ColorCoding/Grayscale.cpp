/*
 * Grayscale.cpp
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#include "Grayscale.h"

#include <math.h>

Grayscale::Grayscale() {
	// TODO Auto-generated constructor stub

}

Grayscale::~Grayscale() {
	// TODO Auto-generated destructor stub
}

void Grayscale::getColor(double d, double *c) {
    d = 1.0 - fabs(d);

    c[0] = d;
    c[1] = d;
    c[2] = d;
}
