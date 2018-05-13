#include <iostream>
#include <algorithm>
#include <deque>
#include <bitset>
#include <vector>
#include <limits>
#include <iomanip>
#include "Platform.h"
#include "ConnectionManager.h"
#include "LogManager.h"
#include "Serialize.h"
#include "BitPacker.h"
#include "BitWriter.h"
#include "BitReader.h"
#include "Stream/ReadStream.h"
#include "Stream/WriteStream.h"
#include "Stream/Stream.h"

#define SERVER_NAME "Server"

int main(int argc, char** argv)
{
	std::cout << "running.." << std::endl;
	
	const int dataSizeInBytes = 256;

	uint8_t data[dataSizeInBytes];

	{
		CWriteStream writeStream(data, dataSizeInBytes);
		float val = 10.000002f;
		std::cout << std::fixed << std::setprecision(6) << val << std::endl;
		serialize_float(writeStream, val);
	}

	float val;
	{
		CReadStream readStream(data, dataSizeInBytes);
		serialize_float(readStream, val);
	}
	
	std::cout << std::fixed << std::setprecision(6) << val << std::endl;

	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP

	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();

	return 0;
}

/*
const int dataSizeInBytes = 8000;
const int maxNumber = 255;
const int numToWrite = dataSizeInBytes;

unsigned char data[dataSizeInBytes];

uint32_t numbers[numToWrite];
uint32_t outputNumbers[numToWrite];

std::srand((unsigned)time(0));

for (int i = 0; i < numToWrite; i++)
{
numbers[i] = (rand() % maxNumber);
}

{
CWriteStream writeStream(data, dataSizeInBytes);

for (int i = 0; i < numToWrite; i++)
{
serialize_int(writeStream, numbers[i], 0, maxNumber);
}
}

{
CReadStream readStream(data, dataSizeInBytes);

for (int i = 0; i < numToWrite; i++)
{
uint32_t num;
serialize_int(readStream, num, 0, maxNumber);
outputNumbers[i] = num;
}
}

for (int i = 0; i < numToWrite; i++)
{
UDP_TRAP(numbers[i] == outputNumbers[i]);
}

std::cout << "Serialization test done." << std::endl;

*/