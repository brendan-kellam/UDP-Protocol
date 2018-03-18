#include "BitPacker.h"
#include "Platform.h"
#include <string>

CBitPacker::CBitPacker(uint32_t* buffer, size_t bufferLen)
	: m_scratch(0),
	m_scratchBits(0),
	m_wordIndex(0),
	m_buffer(buffer),
	m_bufferLen(bufferLen)
{
	UDP_TRAP(bufferLen % 4 == 0)
}


std::string CBitPacker::getBits(uint64_t val)
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
