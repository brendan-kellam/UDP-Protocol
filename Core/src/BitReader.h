#ifndef BITREADER_H
#define BITREADER_H

#include <iostream>
#include <sstream>

#define WORD_SIZE 32


class CBitReader
{

private:
	// 64bit scratch word: Double the word size (32bits)
	// Used for overflow when writing
	uint64_t m_scratch;

	// The current number of bits currently in m_scratch
	int m_scratchBits;

	// Number of words
	size_t m_totalBits;

	int m_numBitsRead;

	int m_wordIndex;


	// Buffer pointer
	// NOTE: Packet payload buffer needs to be a multiple of 4 bytes
	// (Due to word level = 32 bits)
	uint32_t* m_buffer;

	// Length of m_buffer
	size_t m_bufferLen;

	std::ostringstream log;

	std::string getBits(uint64_t val);

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