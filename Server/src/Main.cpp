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

	unsigned char data[56];

	
	{
		CBitWriter writer((uint32_t*)data, sizeof(data));
		for (int i = 0; i < 32; i++)
		{
			writer.WriteBits(10922, 14);
		}
	}
	
	{
		CBitReader reader((uint32_t*)data, sizeof(data));
		for (i = 0; i < 32; i++)
		{
			std::cout << reader.ReadBits(14) << std::endl;
		}
	}
	

	unsigned char test[256];
	std::cout << test[0] << std::endl;

	uint32_t* t = (uint32_t*)test;

	t[256] = UINT32_MAX;

	std::cout << t[255] << std::endl;

	uint32_t a[10];
	std::cout << sizeof(data) << std::endl;

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

