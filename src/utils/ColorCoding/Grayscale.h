/*
 * Grayscale.h
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include "ColorCoder.h"

class Grayscale: public ColorCoder {
public:
	Grayscale();
	virtual ~Grayscale();
	void getColor(double d, double *c);
};

#endif /* GRAYSCALE_H_ */
