#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <bitset>
#include <chrono>
#include "BitWriter.h"
#include "BitReader.h"

///////////////////////////PACKET STRUCTURE///////////////////////////////
//////////////////////////////////////////////////////////////////////////
// [uint protocol id]			{4   bytes}			// -- HEADER START
// [uint sequencing id]			{2   bytes}
// [uint ack]					{2   bytes}
// [uint ack bitfield]			{4   bytes}			// -- HEADER END
// [void* payload]				{244 bytes}
//////////////////////////////////////////////////////////////////////////


// -- PACKET SIZE : 256 bytes
#define PACKET_SIZE 256

// -- PACKET HEADER SIZE : 12 bytes
#define PACKET_HEADER_SIZE 12

// -- PAYLOAD SIZE : 244 bytes
#define PAYLOAD_SIZE PACKET_SIZE-PACKET_HEADER_SIZE



#define INVALID_PACKET_ID 1


class CPacket
{

private: 
	typedef std::chrono::steady_clock::time_point timePoint;

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


	bool ConstructPacket(unsigned char payload[PAYLOAD_SIZE]);
	bool DeconstructPacket();

	void SetSentTime(timePoint time)		{ m_sentTime = time; }
	timePoint GetSentTime() const			{ return m_sentTime; }

	void AckPacket()						{ m_isAcked = true; }
	bool IsAcked() const					{ return m_isAcked; }

	uint16_t GetID() const					{ return m_id; }
	uint16_t GetAck() const					{ return m_ack; }
	uint32_t GetAckBitfieldInt() const		{ return m_ackBitfieldInt; }

	bool operator==(const CPacket & packet) const
	{
		return this->GetID() == packet.GetID();
	}

protected:

	// Sequencing id -- Identifies a specific packet
	uint16_t m_id;

	// ACK -- Remote sequence number for Connection
	uint16_t m_ack;

	// ACK Bitfield - unsigned integer form
	uint32_t m_ackBitfieldInt;

	// ACK Bitfield
	//std::bitset<BITFIELD_SIZE> m_bitfield;

	// SENTTIME - Time this packet is sent
	timePoint m_sentTime;

	// ISACKED - set to TRUE iff this packet has been acked
	bool m_isAcked;

private:

	unsigned char m_buffer[PACKET_SIZE];

	uint32_t ms_protocolID = static_cast<uint32_t>(std::hash<std::string>()("Eros"));


};


const int MaxNumber = 10;
const int MaxNumberBits = BITS_REQUIRED(0, MaxNumber);
struct TestPacket
{

	int myInt;

	void Write(CBitWriter& writer)
	{
		writer.WriteBits(myInt, MaxNumberBits);
	}

	void Read(CBitReader& reader)
	{
		reader.ReadBits(MaxNumberBits);
	}

};


#endif /* PACKET_H */