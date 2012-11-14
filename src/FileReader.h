/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#ifndef FILE_READER_H_
#define FILE_READER_H_

#include "Particle.h"
#include "ParticleContainer.h"

#include <log4cxx/logger.h>

#include <vector>

class FileReader {
private:
	static log4cxx::LoggerPtr logger;

public:
	FileReader();
	virtual ~FileReader();

	void readFile(ParticleContainer& container, char* filename);

};

#endif /* FILE_READER_H_ */
