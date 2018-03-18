#include "BitWriter.h"

#include <string>

// Constructor
CBitWriter::CBitWriter(uint32_t* buffer, size_t bufferLen)
	: m_scratch(0),
	m_scratchBits(0),
	m_wordIndex(0),
	m_bufferLen(bufferLen),
	m_buffer(buffer)
{
	log << "Bit writer running..";
	CLogManager::Instance().WriteLine(log.str());
	log.str("");

	UDP_TRAP(bufferLen % 4 == 0);
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
	UDP_TRAP(m_wordIndex < m_bufferLen);
	UDP_TRAP(m_scratchBits <= WORD_SIZE)
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
	UDP_TRAP(bits <= WORD_SIZE);
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
	if (m_scratchBits > WORD_SIZE)
	{
		// Ensure the word index is strictly less than the buffer index
		UDP_TRAP(m_wordIndex < m_bufferLen);

		// Telem
		{
			log << "---- Overflow! -----";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

		// # of bits overflowed
		int overflowBits = m_scratchBits - WORD_SIZE;

		// Telem
		{
			log << "Overflowed by: " << overflowBits << " bits";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

		// Create mask
		unsigned mask;
		mask = (1 << overflowBits) - 1;

		// From the scratch, grab the bits that overflowed.
		uint32_t overflow = m_scratch & mask;

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
		m_wordIndex++;

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

void printBits(uint64_t val, bool newLine /* = true */)
{

	if (val == 0)
	{
		std::cout << 0;
	}

	std::string g;

	while (val != 0)
	{
		g += std::to_string((val % 2 != 0));

		val /= 2;
	}

	std::reverse(g.begin(), g.end());

	std::cout << g.c_str();
	if (newLine) std::cout << std::endl;
}

std::string getBits(uint64_t val)
{

	if (val == 0)
	{
		return std::string("0");
	}

	std::string g;
	while (val != 0)
	{
		g += std::to_string((val % 2 != 0));

		val /= 2;
	}
	std::reverse(g.begin(), g.end());

	return g;
}