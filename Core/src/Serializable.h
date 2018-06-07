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

#endif 