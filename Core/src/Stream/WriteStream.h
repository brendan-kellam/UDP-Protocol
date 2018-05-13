#ifndef WRITESTREAM_H
#define WRITESTREAM_H

#include "Stream.h"
#include "BitWriter.h"
#include <iostream>

class CWriteStream
	: public Stream::IStream
{

public: 

	enum { IsWriting = 1 };
	enum { IsReading = 0 };

	//************************************
	// Method:    CWriteStream
	// FullName:  CWriteStream::CWriteStream
	// Access:    public 
	// Qualifier: : m_writer((uint32_t*) buffer, bytes)
	// Parameter: uint8_t * buffer
	// Parameter: int bytes
	//************************************
	CWriteStream(uint8_t* buffer, int bytes)
		: m_writer((uint32_t*) buffer, bytes) {}

	bool SerializeInteger(int32_t val, int32_t min, int32_t max);
	bool SerializeBits(uint32_t val, const int bits);

private:
	CBitWriter m_writer;
};


#endif 
