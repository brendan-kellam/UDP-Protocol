#ifndef BITPACKER_H
#define BITPACKER_H

#include <iostream>

#define WORD_SIZE 32

class CBitPacker
{
protected:
	CBitPacker(uint32_t* buffer, size_t bufferLen);

public:
	virtual ~CBitPacker() {};

protected:
	// 64bit scratch word: Double the word size (32bits)
	// Used for overflow when writing
	uint64_t m_scratch;

	// The current number of bits currently in m_scratch
	int m_scratchBits;

	// Current index in m_buffer
	int m_wordIndex;

	// Buffer pointer
	// NOTE: Packet payload buffer needs to be a multiple of 4 bytes
	// (Due to word level = 32 bits)
	uint32_t* m_buffer;

	// Length of m_buffer
	size_t m_bufferLen;

	std::string getBits(uint64_t val);

};


#endif
