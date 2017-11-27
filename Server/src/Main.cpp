#include <iostream>
#include <algorithm>
#include <deque>
#include <bitset>
#include "ConnectionManager.h"
#include "LogManager.h"
#include "Serialize.h"

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

	std::bitset<32> b(27);

	std::cout << b << std::endl;

	for (int i = 0; i < 32; i++)
	{
		if (b[i])
		{
			std::cout << "Bit Set - " << i << std::endl;
		}
	}


	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP


	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();

	return 0;
}

