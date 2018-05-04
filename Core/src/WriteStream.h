#ifndef WRITESTREAM_H
#define WRITESTREAM_H

#include "Stream.h"
#include "BitWriter.h"
#include <iostream>

class CWriteStream
	: public IStream
{

public: 
	CWriteStream(uint8_t* buffer, int bytes)
		: m_writer((uint32_t*) buffer, bytes) {}

	bool SerializeInteger(int32_t val, const int32_t min, const int32_t max);

private:
	CBitWriter m_writer;
};


#endif 
