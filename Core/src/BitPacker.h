#ifndef BITPACKER_H
#define BITPACKER_H

#include <iostream>
#include <math.h>

#define WORD_SIZE_IN_BITS 32
#define WORD_SIZE_IN_BYTES 4

// --- meta programming to determine the # of required bits to represent a decimal number ---
// From: https://gafferongames.com/post/reading_and_writing_packets/
template <uint32_t x> 
struct PopCount
{
	enum {
		a = x - ((x >> 1) & 0x55555555),
		b = (((a >> 2) & 0x33333333) + (a & 0x33333333)),
		c = (((b >> 4) + b) & 0x0f0f0f0f),
		d = c + (c >> 8),
		e = d + (d >> 16),
		result = e & 0x0000003f
	};
};

template <uint32_t x> 
struct Log2
{
	enum {
		a = x | (x >> 1),
		b = a | (a >> 2),
		c = b | (b >> 4),
		d = c | (c >> 8),
		e = d | (d >> 16),
		f = e >> 1,
		result = PopCount<f>::result
	};
};

template <int64_t min, int64_t max> 
struct BitsRequired
{
	static const uint32_t result =
		(min == max) ? 0 : (Log2<uint32_t(max - min)>::result + 1);
};

#define BITS_REQUIRED( min, max ) BitsRequired<min,max>::result
// ---


uint32_t bitsRequired(uint32_t min, uint32_t max);

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
