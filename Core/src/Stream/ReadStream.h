#ifndef READSTREAM_H
#define READSTREAM_H

#include "Stream.h"
#include "BitReader.h"
#include <iostream>

class CReadStream
	: public Stream::IStream
{
public:

	enum { IsWriting = 0 };
	enum { IsReading = 1 };

	CReadStream(uint8_t* buffer, int bytes)
		: m_reader((uint32_t*)buffer, bytes) {}

	bool SerializeInteger(int32_t& val, int32_t min, int32_t max);

	bool test(CReadStream readStream, uint32_t val, uint32_t min, uint32_t max)
	{
		serialize_int(readStream, val, min, max);
	}

private:
	CBitReader m_reader;

};


#endif