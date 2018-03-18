#include "BitReader.h"
#include "LogManager.h"
#include "Platform.h"

CBitReader::CBitReader(uint32_t* buffer, size_t bufferLen)
	: CBitPacker(buffer, bufferLen),
	m_numBitsRead(0),
	m_totalBits(bufferLen * 8)
{
	log << "Bit reader running..";
	CLogManager::Instance().WriteLine(log.str());
	log.str("");
}

uint32_t CBitReader::ReadBits(const int bits)
{
	UDP_TRAP(bits <= WORD_SIZE);
	UDP_TRAP(bits > 0);
	UDP_TRAP(m_numBitsRead <= m_totalBits);
	
	// Telem
	{
		log << "Reading in " << bits << " bits";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// Telem
	{
		log << "Current scratch status: " << m_scratch << " [" << getBits(m_scratch) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	// Read in next word
	if (bits > m_scratchBits)
	{
		// Telem
		{
			log << "----- Reading in next word: " << m_buffer[m_wordIndex] << " [" << getBits(m_buffer[m_wordIndex]) << "] -----";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}

		m_scratch <<= WORD_SIZE;

		// We can ONLY read if the wordIndex is stricly less than the bufferLength
		UDP_TRAP(m_wordIndex < m_bufferLen)

		m_scratch += m_buffer[m_wordIndex];
		m_scratchBits += WORD_SIZE;
		m_wordIndex++;
		
		// Telem
		{
			log << "Scratch: " << m_scratch << " [" << getBits(m_scratch) << "]";
			CLogManager::Instance().WriteLine(log.str());
			log.str("");
		}
	}

	// Telem
	{
		log << "Scratch Bits: " << m_scratchBits;
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}


	int remainingBits = m_scratchBits - bits;

	// Create mask
	unsigned mask;
	mask = (1 << bits) - 1;

	/* 
	* We want to grab the #<bits> most significant bits from m_scratch. This can be done by shifting
	* m_scratch right by the difference between the total number of bits in m_scratch (m_scratchBits) and
	* the number of bits we wish to grab (bits) [remainingBits]
	* Visual example:
	* bits = 3, m_scratchbits = 32, 
	* m_scratch = 11010101011011111010110101010010 [3580865874]
	* mask = 111 (7)
	* m_scratch >> 29 = 110
	* 110 & 111 = 110
	* => output = 110 [6]
	*/
	uint32_t output = (m_scratch >> remainingBits) & mask;
	
	// Grab remaining bits
	mask = (1 << remainingBits) - 1;
	m_scratch &= mask;

	m_scratchBits = remainingBits;

	std::cout << output << std::endl;
	std::cout << m_scratch << std::endl;

	m_numBitsRead += bits;

	// Telem
	{
		log << "Read: " << output << " [" << getBits(output) << "]";
		CLogManager::Instance().WriteLine(log.str());
		log.str("");
	}

	
	return output;
}