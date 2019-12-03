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
#include "BitPacker.h"
#include "BitWriter.h"
#include "BitReader.h"
#include "Stream/ReadStream.h"
#include "Stream/WriteStream.h"
#include "Stream/Stream.h"
#include "Serializable.h"
#include "Util/SequenceBuffer.h"
#include "Message/SimpleMessage/SimpleMessage.h"
#include "Message/Message.h"

#define SERVER_NAME "Server"

int main(int argc, char** argv)
{
	std::cout << "running.." << std::endl;
	
	const int dataSizeInBytes = 256;

	uint8_t data[dataSizeInBytes];

	
	{
		uint8_t transmissionBuf[16];

		CWriteStream writeStream(transmissionBuf, 16);
		CReadStream readStream(transmissionBuf, 16);


		// --- PEER A ---

		{
			CSimpleMessage msgSend;

			uint32_t typeSend = msgSend.GetType();
			serialize_int(writeStream, typeSend, 0, ETypes::LENGTH);

			msgSend.SetMessage(15);
			msgSend.Serialize(writeStream);
			writeStream.Flush();
		}
		
		// --- PEER B ---

		CSimpleMsgFactory facRecv;
		uint32_t typeRecv;
		serialize_int(readStream, typeRecv, 0, ETypes::LENGTH);

		if (auto msgRecvOptional =
			facRecv.CreateMessage(typeRecv))
		{
			std::shared_ptr<CSimpleMessage> msg =
				std::dynamic_pointer_cast<CSimpleMessage>(msgRecvOptional.value());

			msg->Serialize(readStream);
			std::cout << "Message: " << msg->GetMessage() << std::endl;
		}

	}

	CLogManager::Instance().StartUp();
	CConnectionManager::Instance().StartUp();
	
	// MAIN LOOP

	CConnectionManager::Instance().ShutDown();
	CLogManager::Instance().ShutDown();

	return 0;
}