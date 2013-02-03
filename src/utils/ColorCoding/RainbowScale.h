/*
 * RainbowScale.h
 *
 *  Created on: 03.02.2013
 *      Author: j
 */

#ifndef RAINBOWSCALE_H_
#define RAINBOWSCALE_H_

#include "ColorCoder.h"

class RainbowScale: public ColorCoder {
private:
	double huemin, huemax;

public:
	RainbowScale();
	~RainbowScale();
	void getColor(double d, double *c);

};

#endif /* RAINBOWSCALE_H_ */
