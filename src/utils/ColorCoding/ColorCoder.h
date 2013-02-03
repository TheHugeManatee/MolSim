/*
 * ColorCoder.h
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#ifndef COLORCODER_H_
#define COLORCODER_H_

#include <iostream>

class ColorCoder {

public:
	static void HSVToRGB(double h, double s, double v, double *c);

	double min, max, maxminusmin;

	ColorCoder();
	virtual ~ColorCoder();

	virtual void getColor(double d, double *c);

	void setMin(double min);
	void setMax(double max);
};

#endif /* COLORCODER_H_ */
