/*
 * ColorCoder.h
 *
 *  Created on: 17.01.2013
 *      Author: j
 */

#ifndef COLORCODER_H_
#define COLORCODER_H_

class ColorCoder {
protected:
	double min, max;

public:
	ColorCoder();
	virtual ~ColorCoder();

	virtual void getColor(double d, double *c) const = 0;

	void setMin(double min) {
		this->min = min;
	}

	void setMax(double max)  {
		this->max = max;
	}
};

#endif /* COLORCODER_H_ */
