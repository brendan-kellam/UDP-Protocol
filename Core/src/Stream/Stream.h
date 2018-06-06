#ifndef STREAM_H
#define STREAM_H

#include "Platform.h"
#include <math.h>
#include <iostream>
#include <vector>

/*
From Gaffer on Games Article: https://gafferongames.com/post/serialization_strategies/

"To handle safety serialize_* calls are not actually functions at all. 
They're actually macros that return false on error, thus unwinding the 
stack in case of error, without the need for exceptions."
*/

// Serialize integer in given range (min, max)
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
			std::cout << "Serialization issue" << std::endl;		\
			return false;                                           \
		}                                                           \
		if ( stream.IsReading )                                     \
		{                                                           \
			value = int32_value;                                    \
			if ( value < min || value > max )                       \
			{                                                       \
				std::cout << "Serialization range issue" << std::endl;\
				return false;                                       \
			}                                                       \
		}                                                           \
		} while (0)

#define serialize_bits( stream, value, bits )						\
	do																\
	{																\
		uint32_t uint32_value;										\
		if ( Stream::IsWriting )									\
		{															\
			UDP_TRAP( value > 0 );									\
			uint32_value = (uint32_t) value;						\
		}															\
		if ( !stream.SerializeBits( uint32_value, bits ) )			\
		{															\
			return false;											\
		}															\
		if ( Stream::IsReading )									\
		{															\
			value = uint32_value;									\
			if ( value < 0 )										\
			{														\
				return false;										\
			}														\
		}															\
	} while (0)

#define serialize_float( stream, value )							\
	do																\
	{																\
		if ( !serialize_float_internal( stream, value ) )			\
		{															\
			return false;											\
		}															\
		} while(0)


template <typename Stream>
bool serialize_float_internal(Stream& stream, float& value)
{
	union FloatInt
	{
		float float_value;
		uint32_t int_value;
	};

	FloatInt tmp;
	if (stream.IsWriting)
	{
		tmp.float_value = value; 
	}

	bool result = stream.SerializeBits(tmp.int_value, 32);
	
	if (stream.IsReading)
	{
		value = tmp.float_value;
	}

	return result;
}

template <typename Stream>
bool serialize_vector_internal(Stream& stream, std::vector<float>& vector)
{


	return true;
}

namespace Stream
{
	class IStream
	{
	public: 
		virtual ~IStream() {}
	};
}

#endif

