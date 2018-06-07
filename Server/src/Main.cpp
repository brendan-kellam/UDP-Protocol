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
#include "Serializable.h"
#include "Util/SequenceBuffer.h"

#define SERVER_NAME "Server"

int main(int argc, char** argv)
{
	std::cout << "running.." << std::endl;
	
	const int dataSizeInBytes = 256;

	uint8_t data[dataSizeInBytes];

	/*
	{
		uint8_t transmissionBuf[16];

		CWriteStream writeStream(transmissionBuf, 16);
		CReadStream readStream(transmissionBuf, 16);

		CMessage msgSend;
		CMessage msgRecv;

		// --- PEER A ---

		msgSend.SetMessage(7);

		msgSend.Serialize(writeStream);
		writeStream.Flush();
		
		// --- PEER B ---

		msgRecv.Serialize(readStream);

		std::cout << "Message: " << msgRecv.GetMessage() << std::endl;
	}*/

	CSequenceBuffer<int> myBuf(10);

	for (uint16_t i = 0; i < 5; i++)
	{
		int s = 10;
		*myBuf.Insert(i) = s;
	}
	
	std::cout << *myBuf.Find(3) << std::endl;


	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP

	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();

	return 0;
}