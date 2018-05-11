#include <iostream>
#include <algorithm>
#include <deque>
#include <bitset>
#include "ConnectionManager.h"
#include "LogManager.h"
#include "Serialize.h"
#include "BitWriter.h"
#include "BitReader.h"
#include "Stream/ReadStream.h"
#include "Stream/WriteStream.h"
#include "Stream/Stream.h"

#define SERVER_NAME "Server"

unsigned int test = static_cast<unsigned int>(std::hash<std::string>()("Eros"));

void tt(unsigned char* buffer, char a, char b, char c, char d)
{
	buffer[0] = a;
	buffer[1] = b;
	buffer[2] = c;
	buffer[3] = d;
}

void SerializeUInt(unsigned char* buffer, unsigned const int value)
{
	buffer[0] = (value >> 24) & 0xff;
	buffer[1] = (value >> 16) & 0xff;
	buffer[2] = (value >> 8) & 0xff;
	buffer[3] = value & 0xff;
}

void DeserializeUInt(unsigned char* buffer, unsigned int &value)
{
	value = (buffer[0] << 24)
		| (buffer[1] << 16)
		| (buffer[2] << 8)
		| buffer[3];
}

int main(int argc, char** argv)
{
	std::cout << "running.." << std::endl;

	unsigned char data[56];

	const int MaxElements = 32;
	const int MaxElementBits = BITS_REQUIRED(0, MaxElements);

	std::cout << "Bits: " << MaxElementBits << std::endl;
	
	{
		CBitWriter writer((uint32_t*)data, sizeof(data));
		for (int i = 0; i < 2; i++)
		{
			writer.WriteBits(10922, 14);
		}
	}
	
	{
		CBitReader reader((uint32_t*)data, sizeof(data));
		for (int i = 0; i < 2; i++)
		{
			reader.ReadBits(14);
		}
	}
	

	unsigned char test[256];
	std::cout << test[0] << std::endl;

	uint32_t* t = (uint32_t*)test;

	t[256] = UINT32_MAX;

	std::cout << t[255] << std::endl;

	uint32_t a[10];
	std::cout << sizeof(data) << std::endl;

	uint8_t buf[4];

	uint32_t val = 2;
	uint32_t minimum = 0;
	uint32_t maximum = 10;
	
	/*
	{
		CWriteStream writeStream(buf, sizeof(buf));
		serialize_int(writeStream, val, minimum, maximum);
	}
	uint32_t result;

	{
		CReadStream readStream(buf, sizeof(buf));
		serialize_int(readStream, result, minimum, maximum);
	}

	std::cout << "RESULT: " << result << std::endl;

	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP


	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();
	*/

	return 0;
}

