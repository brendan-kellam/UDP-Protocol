#ifndef BITREADER_H
#define BITREADER_H

#include <sstream>
#include "BitPacker.h"

class CBitReader 
	: public CBitPacker
{

private:

	// Number of words
	size_t m_totalBits;

	// Total of number bits read
	int m_numBitsRead;

	std::ostringstream log;

public:


	//************************************
	// Method:    BitWriter
	// FullName:  CBitWriter::BitWriter
	// Access:    public 
	// Parameter: uint32_t * buffer - Data buffer
	// Parameter: size_t bufferLen - Size of buffer (IN BYTES)
	//************************************
	CBitReader(uint32_t* buffer, size_t bufferLen);


	uint32_t ReadBits(const int bits);

};

#endif