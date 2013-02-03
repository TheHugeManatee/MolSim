/*
 * ColorCoder.cpp
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#include "ColorCoder.h"
#include <math.h>

ColorCoder::ColorCoder(): min(0), max(1), maxminusmin(1) {


}

ColorCoder::~ColorCoder() {
	// TODO Auto-generated destructor stub
}

void ColorCoder::getColor(double d, double *c) {
	c[0] = c[1] = c[2] = 0.0;
}


void ColorCoder::HSVToRGB(double h, double s, double v, double *c) {
    int i;
    double f, p, q, t,hTemp;
    double r, g, b;

    if ( s == 0.0 || h == -1.0) { // s==0? Totally unsaturated = grey so R,G and B all equal value
        r = g = b = v;
        c[0] = r;
        c[1] = g;
        c[2] = b;
        return;
    }
    hTemp = h/60.0;
    i = (int)floor( hTemp );                 // which sector
    f = hTemp - i;                      // how far through sector
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );

    switch ( i ) {
    case 0: {
        r = v;        g = t;        b = p;
        break;
    }
    case 1: {
        r = q;        g = v;        b = p;
        break;
    }
    case 2: {
        r = p;        g = v;        b = t;
        break;
    }
    case 3: {
        r = p;        g = q;        b = v;
        break;
    }
    case 4: {
        r = t;        g = p;        b = v;
        break;
    }
    case 5: {
        r = v;        g = p;        b = q;
        break;
    }
    }

    c[0] = r;
    c[1] = g;
    c[2] = b;
    return;
}
