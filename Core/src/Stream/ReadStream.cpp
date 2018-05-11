#include "ReadStream.h"
#include "Platform.h"

bool CReadStream::SerializeInteger(int32_t& val, int32_t min, int32_t max)
{
	UDP_TRAP(min < max);

	// Determine the # of required bits
	const int bits = bitsRequired(min, max);

	// Read bits
	uint32_t uval = m_reader.ReadBits(bits);

	val = (int32_t)uval + min;


	return true;
}
