/*
 * HeatedObject.h
 *
 *  Created on: 03.02.2013
 *      Author: j
 */

#ifndef HEATEDOBJECT_H_
#define HEATEDOBJECT_H_

#include "ColorCoder.h"

class HeatedObjectScale: public ColorCoder {
public:
	HeatedObjectScale();
	~HeatedObjectScale();
	void getColor(double d, double *c);
};

#endif /* HEATEDOBJECT_H_ */
