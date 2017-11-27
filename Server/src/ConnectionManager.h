#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <WinSock2.h>

#include "Manager.h"
#include "Singleton.h"
#include "Packet.h"
#include "Socket.h"
#include "Connection.h"
#include "Address.h"


#define ENGINE_NAME		   "Eros"
#define LOCAL_PORT			2000
#define BUFLEN				256




class CConnectionManager final
	: public IManager, public CSingleton<CConnectionManager>
{
public: 

	CConnectionManager();
		
	void StartUp() override;
	void ShutDown() override;

	bool SendPacket(CAddress& to, CPacket& packet);

private:

	CConnection* GetConnection(CAddress& addr);
	void		 UpdateConnections();

	CSocket m_serverSocket;


	// Winsock
	bool InitializeWinsock();


	WSADATA m_wsaData;


	/*typedef std::vector<CConnection*> ConnectionList;
	typedef std::map<unsigned int, ConnectionList> ConnectionMap;

	ConnectionMap m_connections;
*/

	typedef std::map<CAddress, CConnection*, CAddressCompare> ConnectionMap;
	ConnectionMap m_connections;

	

};