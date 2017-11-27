#pragma once

#include <iostream>

namespace Serialize
{

	typedef unsigned char uchar;

	void SerializeU32Int	(uchar* buffer, const uint32_t value);
	void DeserializeU32Int  (uchar* buffer, uint32_t &value);

	void SerializeU16Int	(uchar* buffer, const uint16_t value);
	void DeserializeU16Int	(uchar* buffer, uint16_t &value);

}
