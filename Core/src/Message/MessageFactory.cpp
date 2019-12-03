#include "MessageFactory.h"
#include "Message/Message.h"

CMessageFactory::CMessageFactory(int numTypes) 
	: m_numTypes(numTypes)
{ }

CMessageFactory::~CMessageFactory()
{
}

std::optional<std::shared_ptr<CMessage>> CMessageFactory::CreateMessage(int type)
{
	UDP_TRAP(type >= 0);

	auto msg = CreateMessageI(type);
	if (!msg.has_value())
	
	{
		std::cout << "MessageFactory.cpp: Failed to allocate message!" << std::endl;
	}

	return msg;
}

void CMessageFactory::SetMessageType(CMessage* message, int type)
{
	message->SetType(type);
}
