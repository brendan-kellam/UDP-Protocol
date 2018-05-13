#ifndef BITPACKER_H
#define BITPACKER_H

#include <iostream>
#include <math.h>

#define WORD_SIZE_IN_BITS 32
#define WORD_SIZE_IN_BYTES 4

inline uint32_t popcount(uint32_t x)
{
	const uint32_t a = x - ((x >> 1) & 0x55555555);
	const uint32_t b = (((a >> 2) & 0x33333333) + (a & 0x33333333));
	const uint32_t c = (((b >> 4) + b) & 0x0f0f0f0f);
	const uint32_t d = c + (c >> 8);
	const uint32_t e = d + (d >> 16);
	const uint32_t result = e & 0x0000003f;
	return result;
}

inline uint32_t log2(uint32_t x)
{
	const uint32_t a = x | (x >> 1);
	const uint32_t b = a | (a >> 2);
	const uint32_t c = b | (b >> 4);
	const uint32_t d = c | (c >> 8);
	const uint32_t e = d | (d >> 16);
	const uint32_t f = e >> 1;
	return popcount(f);
}


inline uint32_t bitsRequired(uint32_t min, uint32_t max)
{
	if (min == max) return 0;
	return uint32_t(floor((log2(max - min)))) + 1;
}

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

	std::string getBits(uint64_t val, int bits);

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
