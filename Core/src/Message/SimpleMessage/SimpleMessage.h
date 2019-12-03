#ifndef SIMPLEMESSAGE_H
#define SIMPLEMESSAGE_H

#include "Message/Message.h"
#include "Message/MessageFactory.h"
#include <optional>


enum ETypes
{
	SIMPLE_MESSAGE,
	LENGTH
};

class CSimpleMessage
	: public CMessage
{
public:

	CSimpleMessage()
	{
		m_type = ETypes::SIMPLE_MESSAGE;
	}

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

class CSimpleMsgFactory
	: public CMessageFactory
{

public:

	CSimpleMsgFactory() : CMessageFactory(ETypes::LENGTH) { }

protected:
	virtual CMessage* CreateMessageInternal(int type) override
	{
		CMessage* message;

		switch (type)
		{
		case ETypes::SIMPLE_MESSAGE:

			message = new CSimpleMessage();
			if (!message)
			{
				return NULL;
			}
			SetMessageType(message, ETypes::SIMPLE_MESSAGE);
			return message;

		default:
			return NULL;

		}
	}

	virtual std::optional<std::shared_ptr<CMessage>> CreateMessageI(int type) override
	{
		switch (type)
		{
		case ETypes::SIMPLE_MESSAGE:
			return std::make_shared<CSimpleMessage>();

		default:
			return {};
		}
	}
};


#endif