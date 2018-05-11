#ifndef STREAM_H
#define STREAM_H

#include "Platform.h"

/*
From Gaffer on Games Article: https://gafferongames.com/post/serialization_strategies/

"To handle safety serialize_* calls are not actually functions at all. 
They're actually macros that return false on error, thus unwinding the 
stack in case of error, without the need for exceptions."
*/

#define serialize_int( stream, value, min, max )                    \
	do                                                              \
	{                                                               \
		UDP_TRAP( min < max );                                      \
		int32_t int32_value;                                        \
		if ( stream.IsWriting )                                     \
		{                                                           \
			UDP_TRAP( value >= min );                               \
			UDP_TRAP( value <= max );                               \
			int32_value = (int32_t) value;                          \
		}                                                           \
		if ( !stream.SerializeInteger( int32_value, min, max ) )    \
		{                                                           \
			return false;                                           \
		}                                                           \
		if ( stream.IsReading )                                     \
		{                                                           \
			value = int32_value;                                    \
			if ( value < min || value > max )                       \
			{                                                       \
				return false;                                       \
			}                                                       \
		}                                                           \
		} while (0)

namespace Stream
{
	class IStream
	{ };
}

#endif

