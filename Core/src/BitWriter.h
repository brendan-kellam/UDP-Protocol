#ifndef BITWRITER_H
#define BITWRITER_H

#include <iostream>
#include <sstream>

void printBits(uint64_t val, bool newLine = true);


#define WORD_SIZE 32

#define CREATE_MASK(X) \
do { \
unsigned mask; \
mask = (1 << X) - 1; \
} while (0) \

class CBitWriter
{

private:
	// 64bit scratch word: Double the word size (32bits)
	// Used for overflow when writing
	uint64_t m_scratch;

	// The current number of bits currently in m_scratch
	int m_scratchBits;

	// Number of words
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
	CBitWriter(uint32_t* buffer, size_t bufferLen);


	~CBitWriter();

	void WriteBits(uint32_t data, const int bits);


	//************************************
	// Method:    FlushScratch
	// FullName:  CBitWriter::FlushScratch
	// Access:    public 
	// Returns:   void
	// Notes:	  Flushes any remaining data from scratch into m_buffer
	//************************************
	void FlushScratch();
};


#endif