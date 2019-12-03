#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "Platform.h"
#include <iostream>
#include <vector>
#include <optional>

class CMessage;

/*
Abstract factory pattern.
*/
class CMessageFactory
{
public:
	CMessageFactory(int numTypes);

	virtual ~CMessageFactory();

	std::optional<std::shared_ptr<CMessage>> CreateMessage(int type);

	inline int GetNumTypes() const { return m_numTypes; }

protected:
	void SetMessageType(CMessage* message, int type);
	virtual CMessage* CreateMessageInternal(int type) = 0;

	virtual std::optional<std::shared_ptr<CMessage>> CreateMessageI(int type) = 0;


private:
	int m_numTypes;
	std::vector<std::shared_ptr<CMessage>> messages;
};


#endif