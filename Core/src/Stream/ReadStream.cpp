#include "ReadStream.h"
#include "Platform.h"

bool CReadStream::SerializeInteger(int32_t& val, int32_t min, int32_t max)
{
	UDP_TRAP(min < max);

	// Determine the # of required bits
	const int bits = bitsRequired(min, max);

	if (m_reader.WouldReadPastEnd(bits))
	{
		return false;
	}

	// Read bits
	uint32_t uval;
	SerializeBits(uval, bits);

	// Shift value back into scale
	val = (int32_t)uval + min;

	return true;
}

bool CReadStream::SerializeBits(uint32_t& val, const int bits)
{
	val = m_reader.ReadBits(bits);
	return true;
}
