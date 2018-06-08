#ifndef MESSAGE_H
#define MESSAGE_H

#include "Serializable.h"
#include <iostream>

class CMessage
	: public ISerializable
{

public:
	
	CMessage()
		: m_type(0),
		m_refCount(1)
	{ }

	uint32_t GetType() const { return m_type; }
	int GetRefCount() const { return m_refCount; }
	
	virtual bool Serialize(CWriteStream& stream) override = 0;
	virtual bool Serialize(CReadStream& stream) override = 0;

	virtual std::string ToString() = 0;

protected:
	
	void SetType(uint32_t type) { m_type = type; }

	void Acquire()
	{
		UDP_TRAP(m_refCount > 0);
		m_refCount++;
	}

	void Release()
	{
		UDP_TRAP(m_refCount > 0);
		m_refCount--;
	}

	virtual ~CMessage()
	{
		UDP_TRAP(m_refCount == 0);
	}

private:

	friend class CMessageFactory;

	CMessage(const CMessage& other);

	int m_refCount;
	uint32_t m_type;

};




#endif