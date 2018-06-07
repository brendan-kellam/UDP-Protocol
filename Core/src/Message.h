#ifndef MESSAGE_H
#define MESSAGE_H

#include "Serializable.h"
#include <iostream>

class CMessage
	: public ISerializable
{

public:

	virtual bool Serialize(CWriteStream& stream) override = 0;
	virtual bool Serialize(CReadStream& stream) override = 0;

	virtual std::string ToString() = 0;

};

class CSimpleMessage
	: public CMessage
{
private:

	int m_message; // min = 0, max = 1000  

	template <typename Stream>
	bool SerializeInternal(Stream& stream)
	{
		serialize_int(stream, m_message, 0, 1000);

		return true;
	}

public:
	virtual bool Serialize(CWriteStream& stream) override
	{
		return SerializeInternal(stream);
	}

	virtual bool Serialize(CReadStream& stream) override
	{
		return SerializeInternal(stream);
	}

	virtual std::string ToString() override
	{
		return std::to_string(GetMessage());
	}

	void SetMessage(const int m)
	{
		m_message = m;
	}

	int GetMessage() const
	{
		return m_message;
	}
};


#endif