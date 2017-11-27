
#include "Address.h"

CAddress::CAddress()
	: CAddress(127, 0, 0, 1, 0)
{

}

CAddress::CAddress(unsigned char a, 
	unsigned char b, 
	unsigned char c, 
	unsigned char d, 
	unsigned short port)
	: m_port(port),
	m_a(a),
	m_b(b),
	m_c(c),
	m_d(d)
{
	// de-serialize from string -> uint
	m_address = (m_a << 24) |
				(m_b << 16) |
				(m_c << 8)  |
				m_d;

	
}


CAddress::CAddress(unsigned int address, unsigned short port)
{
	m_address = address;
	m_port = port;

	m_a = (m_address >> 24) & 0xff;
	m_b = (m_address >> 16) & 0xff;
	m_c = (m_address >> 8) & 0xff;
	m_d = m_address & 0xff;
}





