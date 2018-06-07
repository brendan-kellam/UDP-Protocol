#define _WINSOCK_DEPRECATED_NO_WARNINGS // Supress winsock warnings

#include "ConnectionManager.h"
#include "LogManager.h"
#include "Packet.h"
#include "Message.h"
#include <conio.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


CConnectionManager::CConnectionManager()
{

}

bool CConnectionManager::InitializeWinsock()
{
	return WSAStartup(MAKEWORD(2, 2), &m_wsaData) == NO_ERROR; 

}


bool CConnectionManager::SendPacket(CAddress& to, CPacket& packet)
{
	// Send packet!
	return m_serverSocket.Send(to, packet.GetBuffer(), PACKET_SIZE);
}

bool isKeyPressed(unsigned timeout_ms = 0)
{
	return WaitForSingleObject(
		GetStdHandle(STD_INPUT_HANDLE),
		timeout_ms) == WAIT_OBJECT_0;
}

void CConnectionManager::StartUp()
{
	
	// --- Initialize winsock
	std::cout << "\nInitialising Winsock...";
	if (!InitializeWinsock())
	{
		std::cout << "Failed. Error code : " << WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout << "Initialized." << std::endl;
	// ----------------------
	
	if (!m_serverSocket.Open(LOCAL_PORT))
	{
		std::cout << "Failed to create socket! Error code : " << WSAGetLastError() << std::endl;
	}
	
	// DO-NOT USE SIZEOF WHEN DETERMINING SIZE OF ARRAY
	
	unsigned char reply[PAYLOAD_SIZE];
	memset(reply, '\0', PAYLOAD_SIZE);

	memcpy(reply, "This is a reply ping!", 22);

	CSimpleMessage replyMessage;
	replyMessage.SetMessage(1);

	CSimpleMessage incomingMessage;

	std::ostringstream log;

	void* tempBuf[PACKET_SIZE];

	// receive packets
	// TODO: Make this shit more elegant. I.E Do NOT use _kbhit
	while (!_kbhit())
	{

		CAddress from;

		// NON-BLOCKING CALL
		int bytesRead = m_serverSocket.Receive(from, tempBuf, PACKET_SIZE);

		if (bytesRead > 0)
		{

			CConnection* connection;

			
			// ---- EXISTING CONNECTION ----
			if (connection = GetConnection(from))
			{
				connection->Receive(incomingMessage, (unsigned char*) tempBuf, PACKET_SIZE);
				connection->Send(replyMessage);
			}

			// ---- NEW CONNECTION ----
			else
			{
				// Create new connection and insert into hashmap
				// TODO: Don't allocate new objects for potential packets not addressed to us!!
				connection = new CConnection(from, m_serverSocket);

				// Check if packet can be received without error
				if (!connection->Receive(incomingMessage, (unsigned char*)tempBuf, PACKET_SIZE))
				{
					delete connection;

					// Ignore connection
					continue;
				}

				m_connections.insert(ConnectionMap::value_type(from, connection));

				connection->Send(replyMessage);
			}

		}

		memset(tempBuf, '\0', PACKET_SIZE);

		UpdateConnections();
	}

}

void CConnectionManager::UpdateConnections()
{
	for (ConnectionMap::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		// Update all connections in HashMap
		CConnection* connection = it->second; 
		connection->Update();
	}
}

CConnection* CConnectionManager::GetConnection(CAddress& addr)
{
	auto iter = m_connections.find(addr);
	if (iter != m_connections.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void CConnectionManager::ShutDown()
{

	// Delete all connections
	while (m_connections.begin() != m_connections.end())
	{
		delete m_connections.begin()->second;
		m_connections.erase(m_connections.begin());
	}

	WSACleanup();
}


