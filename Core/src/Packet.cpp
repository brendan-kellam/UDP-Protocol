#include "Packet.h"
#include "Serialize.h"

CPacket::CPacket(uint16_t lsn, uint16_t rsn, uint32_t pab)
	: m_protocolID(ms_protocolID),
	m_id(lsn),
	m_ack(rsn),
	m_ackBitfieldInt(pab),
	m_isAcked(false)
{
}

CPacket::CPacket()
	: m_isAcked(false)
{

}


bool CPacket::ConstructPacket(unsigned char payload[PAYLOAD_SIZE])
{
	memset(m_buffer, '\0', PACKET_SIZE);
	uint32_t point = 0;

	////////////////////// HEADER SECTION ////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/*
	*	[PROTOCOL ID: 4 BYTES]
	*/
	Serialize::SerializeU32Int(&m_buffer[point], CPacket::ms_protocolID);
	point += sizeof(CPacket::ms_protocolID);

	/*
	*	[PACKET SEQUENCE NUMBER: 2 BYTES]
	*/
	Serialize::SerializeU16Int(&m_buffer[point], m_id);
	point += sizeof(m_id);

	/*
	*	[PACKET ACK (REMOTE SEQUENCE NUMBER): 2 BYTES]
	*/
	Serialize::SerializeU16Int(&m_buffer[point], m_ack);
	point += sizeof(m_ack);

	/*
	*	[PACKET ACK BITFIELD INTEGER: 4 BYTES]
	*/
	Serialize::SerializeU32Int(&m_buffer[point], m_ackBitfieldInt);
	point += sizeof(m_ackBitfieldInt);
	//////////////////////////////////////////////////////////////////////////

	/*
	*	[PAYLOAD]
	*/
	memcpy(&m_buffer[point], payload, PAYLOAD_SIZE);

	return true;
}

bool CPacket::DeconstructPacket()
{
	uint32_t point = 0;

	////////////////////// HEADER SECTION ////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	/*
	*	[REMOTE PROTOCOL ID: 4 BYTES]
	*/
	uint32_t remoteProtocolId;
	Serialize::DeserializeU32Int(&m_buffer[point], remoteProtocolId);
	point += sizeof(remoteProtocolId);

	/*
	*	[PACKET SEQUENCE NUMBER: 2 BYTES]
	*/
	Serialize::DeserializeU16Int(&m_buffer[point], m_id);
	point += sizeof(m_id);

	/*
	*	[PACKET ACK (REMOTE SEQUENCE NUMBER): 2 BYTES]
	*/
	Serialize::DeserializeU16Int(&m_buffer[point], m_ack);
	point += sizeof(m_ack);
		
	/*
	*	[PACKET ACK BITFIELD INTEGER: 4 BYTES]
	*/
	Serialize::DeserializeU32Int(&m_buffer[point], m_ackBitfieldInt);
	point += sizeof(m_ackBitfieldInt);

	//////////////////////////////////////////////////////////////////////////


	// Return true if protocol ID's match
	return CPacket::ms_protocolID == remoteProtocolId;
}


template <typename Stream>
bool CPacket::SerializeInternal(Stream& stream)
{
	serialize_bits(stream, m_protocolID, 32);
	serialize_bits(stream, m_id, 16);
	serialize_bits(stream, m_ack, 16);
	serialize_bits(stream, m_ackBitfieldInt, 32);

	return true;
}


bool CPacket::Serialize(CReadStream& stream)
{
	return SerializeInternal(stream);
}

bool CPacket::Serialize(CWriteStream& stream)
{
	return SerializeInternal(stream);
}
