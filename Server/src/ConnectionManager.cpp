#define _WINSOCK_DEPRECATED_NO_WARNINGS // Supress winsock warnings

#include "ConnectionManager.h"
#include "LogManager.h"
#include "Packet.h"
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


	std::ostringstream log;


	// receive packets
	// TODO: Make this shit more elegent. I.E Do NOT use _kbhit
	while (!_kbhit())
	{

		CAddress from;

		// TODO: Consider moving this out of loop (Construction of a new object is wasteful. 
		// We could instead pass-by-copy to a given connection when a packet arrives)
		CPacket inPacket;

		// NON-BLOCKING CALL
		int bytesRead = m_serverSocket.Receive(from, inPacket.GetBuffer(), PACKET_SIZE);

		if (bytesRead > 0)
		{

			// TODO: Consider moving DeconstructPacket to a given connection (for clarity)
			if (!inPacket.DeconstructPacket())
			{
				log << "{Unknown Packet} (" << from.GetFormattedAddress() << ":" << from.GetPort() << ")";
				CLogManager::Instance().WriteLine(log.str());
				log.str("");
				continue;
			}

			CConnection* connection;

			
			// ---- EXISTING CONNECTION ----
			if (connection = GetConnection(from))
			{
				connection->ReceivePacket(inPacket);
				connection->Send(reply);
			}

			// ---- NEW CONNECTION ----
			else
			{
				// Create new connection and insert into hashmap
				connection = new CConnection(from, m_serverSocket);
				m_connections.insert(ConnectionMap::value_type(from, connection));

				connection->ReceivePacket(inPacket);
				connection->Send(reply);
			}

			std::cout << "Message: " << inPacket.GetPayload() << std::endl;

		}

		memset(inPacket.GetBuffer(), '\0', PACKET_SIZE);

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


