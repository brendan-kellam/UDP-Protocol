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
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main()
{
	std::cout << "Client running.." << std::endl;


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
	int dur = (rand() % 500) + 1;

	void* tempBuf[PACKET_SIZE];

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
				if (!myConnection.Receive((unsigned char*)tempBuf, PACKET_SIZE))
				{
					continue;
				}

			}


			if (std::getline(file, line))
			{
				memcpy(payload, line.c_str(), line.length());

				std::cout << line.c_str() << std::endl;
				
				if (count % 5 == 0)
				{
					// Send packet with SPL
					myConnection.Send(payload, true);
				}
				else
				{
					myConnection.Send(payload);
				}

				memset(payload, '\0', PAYLOAD_SIZE);
			}
			else
			{
				file.close();
				break;
			}


			last = now;
			count++;
			dur = (rand() % 500) + 1;
		}
	}

	/*
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::cout << line << '\n';
		}
		file.close();
	}


	std::cout << "Message #: ";

	int count;
	scanf_s("%d\n", &count);

	

	for (int i = 0; i < count; i++)
	{
		fgets((char *)payload, PAYLOAD_SIZE, stdin);
		myConnection.Send(payload);
		myConnection.Update();

		int bytesRead = sock.Receive(from, packet.GetBuffer(), PACKET_SIZE);

		if (bytesRead)
		{

			if (!packet.DeconstructPacket()) continue;

			myConnection.ReceivePacket(packet);

		}

	}
	*/


	CLogManager::Instance().ShutDown();



	system("pause");


	return 0;
}