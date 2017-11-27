#pragma once

#include <string>
#include <iostream>
#include <sstream>

class CAddress
{

public: 
	CAddress();

	CAddress(unsigned char a,
			unsigned char b,
			unsigned char c,
			unsigned char d,
			unsigned short port);

	CAddress(unsigned int address, unsigned short port);


	// equals operator
	bool operator== (const CAddress& other)
	{
		return (m_address == other.GetAddress() &&
			m_port == other.GetPort());
	}


	unsigned int GetAddress() const
	{
		return m_address;
	}

	std::string GetFormattedAddress()
	{
		BuildFormattedAddress();
		return m_faddress;
	}

	unsigned short GetPort() const
	{
		return m_port;
	}

	void SetAddress(unsigned int address)
	{
		m_address = address;
	}

	void SetPort(unsigned short port)
	{
		m_port = port;
	}


private:

	void BuildFormattedAddress()
	{
		std::ostringstream builder;
		builder << +m_a << "." <<  +m_b << "." << +m_c << "." << +m_d; // '+' promotes x to a type printable as a number, regardless of type
		m_faddress = builder.str();
	}

	unsigned int m_address;
	unsigned short m_port;

	unsigned char m_a, m_b, m_c, m_d;

	
	// Formated address 
	std::string m_faddress;

};


struct CAddressCompare
{
	bool operator() (const CAddress& lhs, const CAddress& rhs) const
	{
		if (lhs.GetAddress() < rhs.GetAddress()) { return true; }
		if (lhs.GetAddress() > rhs.GetAddress()) { return false; }
		return (lhs.GetPort() > rhs.GetPort());
	}
};