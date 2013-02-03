/*
 * MagentaScale.h
 *
 *  Created on: 03.02.2013
 *      Author: j
 */

#ifndef MAGENTASCALE_H_
#define MAGENTASCALE_H_

#include "ColorCoder.h"

class MagentaScale : public ColorCoder {
public:
	MagentaScale();
	~MagentaScale();
	void getColor(double d, double *c);
};

#endif /* MAGENTASCALE_H_ */
