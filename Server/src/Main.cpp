#include <iostream>
#include <algorithm>
#include <deque>
#include <bitset>
#include "ConnectionManager.h"
#include "LogManager.h"
#include "Serialize.h"
#include "BitWriter.h"
#include "BitReader.h"

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

	unsigned char data[256];


	{
		CBitWriter writer((uint32_t*)data, 256);

		writer.WriteBits(6, 3);

		writer.WriteBits(685, 10);

		writer.WriteBits(16099933, 24);

		writer.WriteBits(685, 10);

		writer.WriteBits(174764, 18);
	}

	{
		CBitReader reader((uint32_t*)data, 256);

		reader.ReadBits(3);

		reader.ReadBits(10);

		reader.ReadBits(24);

		reader.ReadBits(10);

		reader.ReadBits(18);

		reader.ReadBits(10);
	}

	/*
	uint32_t* test = (uint32_t*) data;

	for (int i = 0; i < 3; i++)
	{
		std::cout << "Word " << i << ":" << test[i] << std::endl;
	}
	*/

	system("pause");

	/*
	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP


	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();
	*/

	return 0;
}

