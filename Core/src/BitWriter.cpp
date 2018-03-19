#include "BitWriter.h"

#include <string>
#include "LogManager.h"
#include "Platform.h"

CBitWriter::CBitWriter(uint32_t* buffer, size_t bufferLen)
	: CBitPacker(buffer, bufferLen),
	m_numBytesWritten(0)
{
	log << "Bit writer running..";
	CLogManager::Instance().WriteLine(log.str());
	log.str("");
}

CBitWriter::~CBitWriter()
{
	FlushScratch();
}


void CBitWriter::FlushScratch()
{
	// Telem
	{
		log << "Flushing remaining m_scratch data...";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// We can ONLY flush the remaining
	UDP_TRAP(m_numBytesWritten < m_bufferLen);
	UDP_TRAP(m_scratchBits <= WORD_SIZE_IN_BITS)
	UDP_TRAP(m_scratchBits > 0);

	// Telem
	{
		log << "Writing data int: " << m_scratch << " [" << getBits(m_scratch) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// Write remaining data to buffer
	m_buffer[m_wordIndex] = ((uint32_t)m_scratch);
	m_wordIndex++;

}


void CBitWriter::WriteBits(uint32_t data, const int bits)
{
	UDP_TRAP(bits <= WORD_SIZE_IN_BITS);
	UDP_TRAP(bits > 0);


	// Telem
	{
		log << "Writing data int: " << data << " [" << getBits(data) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// Shift 64-bit scratch integer left by 'bits'
	m_scratch <<= bits;

	// Add data to scratch integer
	m_scratch += data;

	// Increase number of bits for scratchBits
	m_scratchBits += bits;
	
	// Telem
	{
		log << "Current status of scratch: " << m_scratch << " [" << getBits(m_scratch) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");

		log << "Scratch Bits: " << m_scratchBits;
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// If a overflow is detected
	if (m_scratchBits > WORD_SIZE_IN_BITS)
	{
		// Ensure the word index is strictly less than the buffer index
		UDP_TRAP(m_numBytesWritten < m_bufferLen);

		// Telem
		{
			log << "---- Overflow! -----";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

		// # of bits overflowed
		int overflowBits = m_scratchBits - WORD_SIZE_IN_BITS;

		// Telem
		{
			log << "Overflowed by: " << overflowBits << " bits";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

		// Create mask
		uint64_t mask;
		mask = (1ULL << overflowBits) - 1ULL;

		// From the scratch, grab the bits that overflowed.
		uint64_t overflow = m_scratch & mask;

		// Shift the scratch "overflowBits" to the right, thus removing the
		// overflow for safe casting to a 32 bit number
		m_scratch >>= overflowBits;

		// Telem
		{
			log << "Writing " << m_scratch << " [" << getBits(m_scratch) << "] to m_buffer";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}
		
		m_buffer[m_wordIndex] = ((uint32_t)m_scratch);
		
		// Increment word index and the number of bytes written
		m_wordIndex++;
		m_numBytesWritten += WORD_SIZE_IN_BYTES;

		m_scratch = overflow;
		m_scratchBits = overflowBits;
			
		// Telem
		{
			log << "New status of scratch: " << m_scratch << " [" << getBits(m_scratch) << "]";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
			log << "-----------";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

	}

		
}