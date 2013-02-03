/*
 * Grayscale.cpp
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#include "Grayscale.h"

#include <math.h>
#include <iostream>

Grayscale::Grayscale() {
	// TODO Auto-generated constructor stub

}

Grayscale::~Grayscale() {
	// TODO Auto-generated destructor stub
}

void Grayscale::getColor(double d, double *c) {
    double k = (d-min)/(maxminusmin);

    c[0] = k;
    c[1] = k;
    c[2] = k;

}
