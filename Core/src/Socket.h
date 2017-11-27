#pragma once

#include <WinSock2.h>

#include "Address.h"


class CSocket
{
public:
	CSocket();
	~CSocket();

	bool Open(unsigned short port);
	void Close();

	bool IsOpen() const; 

	bool Send(const CAddress& destination, const void* data, int size);

	int Receive(CAddress& sender, void* data, int size);

private:
	SOCKET m_handle;

	struct sockaddr_in m_otherAddress;
};