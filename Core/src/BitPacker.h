#ifndef BITPACKER_H
#define BITPACKER_H

#include <iostream>

#define WORD_SIZE_IN_BITS 32
#define WORD_SIZE_IN_BYTES 4

class CBitPacker
{
protected:
	//************************************
	// Method:    CBitPacker
	// FullName:  CBitPacker::CBitPacker
	// Access:    protected 
	// Returns:   
	// Qualifier:
	// Parameter: uint32_t * buffer - Buffer to read/write to
	// Parameter: size_t bufferLen - Length of buffer (IN BYTES)
	//************************************
	CBitPacker(uint32_t* buffer, size_t bufferLen);

public:
	virtual ~CBitPacker() {};

protected:
	// 64bit scratch word: Double the word size (32bits)
	// Used for overflow when writing
	uint64_t m_scratch;

	// The current number of bits currently in m_scratch
	int m_scratchBits;

	// The current index in uin32_t* buffer
	// NOTE: Steps in m_wordIndex represent 4 byte steps in m_buffer
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
