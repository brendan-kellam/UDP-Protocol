#include "Serialize.h"

namespace Serialize
{

	void SerializeU32Int(uchar* buffer, const uint32_t value)
	{
		buffer[0] = (value >> 24) & 0xff;
		buffer[1] = (value >> 16) & 0xff;
		buffer[2] = (value >> 8) & 0xff;
		buffer[3] = value & 0xff;
	}

	void DeserializeU32Int(uchar* buffer, uint32_t &value)
	{
		value = (buffer[0] << 24)
			| (buffer[1] << 16)
			| (buffer[2] << 8)
			| buffer[3];
	}


	void SerializeU16Int(uchar* buffer, const uint16_t value)
	{
		buffer[0] = (value >> 8) & 0xff;
		buffer[1] = value & 0xff;
	}

	void DeserializeU16Int(uchar* buffer, uint16_t &value)
	{
		value = (buffer[0] << 8)
			| buffer[1];
	}

}
