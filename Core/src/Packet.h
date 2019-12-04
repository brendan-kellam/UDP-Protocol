#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <bitset>
#include <chrono>
#include "BitWriter.h"
#include "BitReader.h"
#include "Serializable.h"

///////////////////////////PACKET STRUCTURE///////////////////////////////
//////////////////////////////////////////////////////////////////////////
// [uint protocol id]			{4   bytes}			// -- HEADER START
// [uint sequencing id]			{2   bytes}
// [uint ack]					{2   bytes}
// [uint ack bitfield]			{4   bytes}
// [uint num messages]			{1   byte }			// -- HEADER END
// [void* payload]				{244 bytes}
//
//
//	bytes | name
//	__________________________ Packet header
//	4	 | protocolID
//	2	 | packet id
//	2	 | ack
//	4	 | ack bitfield
//	1    | # messages
//	__________________________ Message 1
//	2    | Message type
//	?    | Message contents
// .....
//	__________________________ Message n
//	2    | Message type
//	?    | Message contents
//	__________________________ Payload
//	?    | Payload contents
//////////////////////////////////////////////////////////////////////////


#define PACKET_SIZE 256
#define PACKET_HEADER_SIZE 13		
#define PAYLOAD_SIZE PACKET_SIZE-PACKET_HEADER_SIZE



#define INVALID_PACKET_ID 1


class CPacket
	: public ISerializable
{

private: 
	typedef std::chrono::steady_clock::time_point timePoint;

	template <typename Stream>
	bool SerializeInternal(Stream& stream);

public:

	// PARAMS: 
	// 1. Local sequence number				(uint 16)
	// 2. Remote sequence number (ACK)		(uint 16)
	// 3. Packet ACK bitfield int			(uint 32)
	CPacket(uint16_t lsn, uint16_t rsn, uint32_t pab);
	
	// Remote packet
	CPacket();

	virtual ~CPacket() { };

	inline unsigned char * GetBuffer() { return m_buffer; }
	inline unsigned char * GetPayload() { return &m_buffer[12]; }

	void SetSentTime(timePoint time)		{ m_sentTime = time; }
	timePoint GetSentTime() const			{ return m_sentTime; }

	void AckPacket()						{ m_isAcked = true; }
	bool IsAcked() const					{ return m_isAcked; }

	uint16_t GetID() const					{ return m_id; }
	uint16_t GetAck() const					{ return m_ack; }
	uint32_t GetAckBitfieldInt() const		{ return m_ackBitfieldInt; }
	uint8_t GetNumMessages() const			{ return m_numMessages; }
	
	bool IsValidProtocolID() const			{ return m_remoteProtocolID == m_protocolID; }

	bool operator==(const CPacket & packet) const
	{
		return this->GetID() == packet.GetID();
	}

	virtual bool Serialize(CReadStream& stream) override;
	virtual bool Serialize(CWriteStream& stream) override;

protected:

	// Sequencing id -- Identifies a specific packet
	uint16_t m_id;

	// ACK -- Remote sequence number for Connection
	uint16_t m_ack;

	// ACK Bitfield - unsigned integer form
	uint32_t m_ackBitfieldInt;

	// Protocol id -- ID for this protocol
	uint32_t m_remoteProtocolID;

	// Num messages - number of messages in this packet
	uint8_t m_numMessages;

	// ACK Bitfield
	//std::bitset<BITFIELD_SIZE> m_bitfield;

	// SENTTIME - Time this packet is sent
	timePoint m_sentTime;

	// ISACKED - set to TRUE iff this packet has been acked
	bool m_isAcked;

private:

	unsigned char m_buffer[PACKET_SIZE];

	uint32_t m_protocolID;


};

#endif /* PACKET_H */