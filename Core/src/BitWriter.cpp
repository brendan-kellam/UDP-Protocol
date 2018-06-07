#include "BitWriter.h"

#include <string>
#include "LogManager.h"
#include "Platform.h"

CBitWriter::CBitWriter(uint32_t* buffer, size_t bufferLen)
	: CBitPacker(buffer, bufferLen),
	m_numBytesWritten(0)
{
#ifdef BITPACKER_DEBUG_LOG
	log << "Bit writer running..";
	CLogManager::Instance().WriteLine(log.str());
	log.str("");
#endif
}

CBitWriter::~CBitWriter()
{ }


void CBitWriter::FlushScratch()
{
#ifdef BITPACKER_DEBUG_LOG
	{
		log << "Flushing remaining m_scratch data...";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}
#endif

	// We can ONLY flush the remaining
	UDP_TRAP(m_numBytesWritten < m_bufferLen);
	UDP_TRAP(m_scratchBits <= WORD_SIZE_IN_BITS)
	UDP_TRAP(m_scratchBits > 0);

#ifdef BITPACKER_DEBUG_LOG
	{
		log << "FLUSH WRITE TO BUFFER: " << m_scratch << " [" << getBits(m_scratch, m_scratchBits) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}
#endif

	// Write remaining data to buffer
	m_buffer[m_wordIndex] = ((uint32_t)m_scratch);
	m_wordIndex++;

}


void CBitWriter::WriteBits(uint32_t data, const int bits)
{
	UDP_TRAP(bits <= WORD_SIZE_IN_BITS);
	UDP_TRAP(bits > 0);


#ifdef BITPACKER_DEBUG_LOG
	{
		log << "WRITE DATA: " << data << " [" << getBits(data, bits) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}
#endif

	uint64_t temp = data;
	temp <<= m_scratchBits;

	// Shift 64-bit scratch integer left by 'bits'
	//m_scratch <<= bits;

	// Add data to scratch integer
	m_scratch ^= temp;

	// Increase number of bits for scratchBits
	m_scratchBits += bits;
	
#ifdef BITPACKER_DEBUG_LOG
	{
		log << "Current status of scratch: " << m_scratch << " [" << getBits(m_scratch, m_scratchBits) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");

		log << "Scratch Bits: " << m_scratchBits;
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}
#endif

	// If a overflow is detected
	if (m_scratchBits > WORD_SIZE_IN_BITS)
	{
		// Ensure the word index is strictly less than the buffer index
		UDP_TRAP(m_numBytesWritten < m_bufferLen);

		// # of bits overflowed
		int overflowBits = m_scratchBits - WORD_SIZE_IN_BITS;

#ifdef BITPACKER_DEBUG_LOG
		{
			log << "Overflowed by: " << overflowBits << " bits";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}
#endif

		uint64_t mask;
		mask = (1ULL << WORD_SIZE_IN_BITS) - 1ULL;

		uint64_t overflow = m_scratch >> WORD_SIZE_IN_BITS;

		m_scratch &= mask;

#ifdef BITPACKER_DEBUG_LOG
		{
			log << "WRITING TO BUFFER: " << m_scratch << " [" << getBits(m_scratch, WORD_SIZE_IN_BITS) << "] to m_buffer";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}
#endif
		
		m_buffer[m_wordIndex] = ((uint32_t)m_scratch);
		
		// Increment word index and the number of bytes written
		m_wordIndex++;
		m_numBytesWritten += WORD_SIZE_IN_BYTES;

		m_scratch = overflow;
		m_scratchBits = overflowBits;
			
#ifdef BITPACKER_DEBUG_LOG
		{
			log << "New status of scratch: " << m_scratch << " [" << getBits(m_scratch, m_scratchBits) << "]";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
			log << "-----------";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}
#endif

	}

		
}