#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "Stream/Stream.h"
#include "Stream/ReadStream.h"
#include "Stream/WriteStream.h"

class ISerializable
{

public:

	virtual ~ISerializable() { }

	virtual bool Serialize(CReadStream& stream) = 0;
	virtual bool Serialize(CWriteStream& stream) = 0;

};

class CMessage 
	: public ISerializable
{
private:

	int m_message; // min = 0, max = 16

	template <typename Stream>
	bool SerializeInternal(Stream& stream)
	{
		serialize_int(stream, m_message, 0, 16);
		
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