#include "WriteStream.h"
#include "Platform.h"
#include <math.h>


bool CWriteStream::SerializeInteger(int32_t val, int32_t min, int32_t max)
{
	UDP_TRAP(min < max);
	UDP_TRAP(val >= min);
	UDP_TRAP(val <= max);

	// Shift to a unsigned value
	uint32_t uval = val - min;
	
	// Determine the # of required bits
	const int bits = bitsRequired(min, max);

	// Write bits
	m_writer.WriteBits(uval, bits);

	return true;
}
