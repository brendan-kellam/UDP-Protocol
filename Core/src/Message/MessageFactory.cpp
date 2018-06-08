#include "MessageFactory.h"
#include "Message/Message.h"

CMessageFactory::CMessageFactory(int numTypes) 
	: m_numTypes(numTypes),
	m_totalRefs(0)
{ }

CMessageFactory::~CMessageFactory()
{
	UDP_TRAP(m_totalRefs == 0);
}

CMessage* CMessageFactory::CreateMessage(int type)
{
	UDP_TRAP(type >= 0);

	CMessage* message = CreateMessageInternal(type);
	
	if (!message)
	{
		std::cout << "MessageFactory.cpp: Failed to allocate message!" << std::endl;
		return NULL;
	}

	m_totalRefs++;
	return message;
}

void CMessageFactory::AcquireMessage(CMessage* message)
{
	UDP_TRAP(message);
	if (message)
	{
		m_totalRefs++;
		message->Acquire();
	}
}

void CMessageFactory::ReleaseMessage(CMessage* message)
{
	UDP_TRAP(message);
	if (!message)
	{
		return;
	}

	message->Release();
	m_totalRefs--;

	if (message->GetRefCount() == 0)
	{
		delete message;
	}
}

void CMessageFactory::SetMessageType(CMessage* message, int type)
{
	message->SetType(type);
}
