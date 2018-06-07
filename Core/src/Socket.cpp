#include "Socket.h"


CSocket::CSocket()
	: m_handle(INVALID_SOCKET)
{

}

CSocket::~CSocket()
{

}

bool CSocket::Open(unsigned short port)
{
	// Check if socket is already open
	if (IsOpen())
	{
		return false;
	}

	// -- Create UDP socket
	m_handle = socket(AF_INET, SOCK_DGRAM, 0);

	if (m_handle == INVALID_SOCKET)
	{
		return false;
	}
	// --------------------

	// -- Bind socket to PORT
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	int err = bind(m_handle, (struct sockaddr*) &address, sizeof(address));

	if (err == SOCKET_ERROR)
	{
		return false;
	}
	// --------------------

	
	// -- Set socket to non blocking
	DWORD nonBlocking = 1;
	err = ioctlsocket(m_handle, FIONBIO, &nonBlocking);
	if (err != 0)
	{
		return false;
	}
	// --------------------

	return true;
}

void CSocket::Close()
{
	closesocket(m_handle);
	m_handle = INVALID_SOCKET;
}

bool CSocket::IsOpen() const
{
	return m_handle != INVALID_SOCKET;
}

bool CSocket::Send(const CAddress& destination, const void* data, int size)
{
	if (m_handle == INVALID_SOCKET)
	{
		return false;
	}

	// -- Setup to address
	memset((char *)&m_otherAddress, 0, sizeof(m_otherAddress));
	m_otherAddress.sin_family = AF_INET;
	m_otherAddress.sin_addr.s_addr = htonl(destination.GetAddress());
	m_otherAddress.sin_port = htons(destination.GetPort());
	int len = sizeof(m_otherAddress);
	// -------------------

	// Send
	int err = sendto(m_handle, (char *)data, size, 0, (struct sockaddr*) &m_otherAddress, len);

	if (err == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}


int CSocket::Receive(CAddress& sender, void* data, int size)
{
	if (m_handle == INVALID_SOCKET)
	{
		return false;
	}

	memset((char *)&m_otherAddress, 0, sizeof(m_otherAddress));
	m_otherAddress.sin_family = AF_INET;
	int len = sizeof(m_otherAddress);

	// Recieve
	int bytes = recvfrom(m_handle, (char*)data, size, 0, (struct sockaddr*) &m_otherAddress, &len);

	sender.SetAddress(ntohl(m_otherAddress.sin_addr.s_addr));
	sender.SetPort(ntohs(m_otherAddress.sin_port));
		

	return bytes;
}





