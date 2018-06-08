#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "Platform.h"
#include <iostream>

class CMessage;

class CMessageFactory
{
public:
	CMessageFactory(int numTypes);

	virtual ~CMessageFactory();

	CMessage* CreateMessage(int type);
	
	void AcquireMessage(CMessage* message);
	void ReleaseMessage(CMessage* message);

	int GetNumTypes() const { return m_numTypes; }

protected:
	void SetMessageType(CMessage* message, int type);
	virtual CMessage* CreateMessageInternal(int type) = 0;


private:
	int m_numTypes;
	int m_totalRefs;

};


#endif