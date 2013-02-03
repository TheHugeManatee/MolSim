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
protected:
	double min, max, maxminusmin;

public:
	ColorCoder();
	virtual ~ColorCoder();

	virtual void getColor(double d, double *c);

	void setMin(double min) {
		this->min = min;
		maxminusmin = max - min;
	}

	void setMax(double max)  {
		this->max = max;
		maxminusmin = max - min;
	}
};

#endif /* COLORCODER_H_ */
