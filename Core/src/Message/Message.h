#ifndef MESSAGE_H
#define MESSAGE_H

#include "Serializable.h"
#include <iostream>

class CMessage
	: public ISerializable
{

public:
	
	CMessage()
		: m_type(0)
	{ }

	uint32_t GetType() const { return m_type; }
	
	virtual bool Serialize(CWriteStream& stream) override = 0;
	virtual bool Serialize(CReadStream& stream) override = 0;

	virtual std::string ToString() = 0;

protected:
	
	void SetType(uint32_t type) { m_type = type; }

	virtual ~CMessage()
	{
	}

	uint32_t m_type;

private:

	friend class CMessageFactory;
	CMessage(const CMessage& other);
};


#endif