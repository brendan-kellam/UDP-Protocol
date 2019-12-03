#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib> 
#include <ctime> 
#include "LogManager.h"
#include "Connection.h"
#include "Address.h"
#include "Socket.h"
#include "Message/Message.h"
#include "Message/SimpleMessage/SimpleMessage.h"
#include <WinSock2.h>
#include "Window.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	std::cout << "Client running.." << std::endl;

	Window window(800, 600, "Test!");


	CLogManager::Instance().StartUp();

	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	// MAIN LOOP
	CSocket sock;
	sock.Open(4002);

	
	CAddress addr(127, 0, 0, 1, 2000);
	CConnection myConnection(addr, sock);

	unsigned char payload[PAYLOAD_SIZE];
	memset(payload, '\0', PAYLOAD_SIZE);
	
	
	CAddress from;

	std::ifstream file("input.txt");
	std::string line;

	auto last = std::chrono::steady_clock::now();
	int count = 0;
	
	std::srand((unsigned)time(0));
	int dur = (rand() % 200) + 1;

	void* tempBuf[PACKET_SIZE];

	CSimpleMessage replyMessage;

	CSimpleMsgFactory messageFactory;

	while (true)
	{
		auto now = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (now - last).count();

		myConnection.Update();

		// 4 ups/s
		if (duration > dur)
		{

			int bytesRead = sock.Receive(from, tempBuf , PACKET_SIZE);
			if (bytesRead > 0)
			{

				// Check if packet can be received without error
				if (!myConnection.Receive(replyMessage, (unsigned char*)tempBuf, PACKET_SIZE))
				{
					continue;
				}

			}


			if (std::getline(file, line))
			{
				memcpy(payload, line.c_str(), line.length());

				std::cout << line.c_str() << std::endl;
				
				if (std::optional<std::shared_ptr<CMessage>> sendMessageOp = messageFactory.CreateMessage(ETypes::SIMPLE_MESSAGE))
				{

					std::shared_ptr<CSimpleMessage> sendMessage =
						std::dynamic_pointer_cast<CSimpleMessage>(sendMessageOp.value());


					sendMessage->SetMessage(count);

					//if (count % 5 == 0)
					//{
					//	// Send packet with SPL
					//	myConnection.Send(*sendMessage, true);
					//}
					//else
					//{
						myConnection.Send(*sendMessage);
					//}

					memset(payload, '\0', PAYLOAD_SIZE);
				}

			}
			else
			{
				file.close();
				break;
			}


			last = now;
			count++;
			dur = (rand() % 500) + 1;

			memset(tempBuf, '\0', PACKET_SIZE);
		}
	}

	CLogManager::Instance().ShutDown();



	system("pause");


	return 0;
}

