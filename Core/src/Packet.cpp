#include "Packet.h"


CPacket::CPacket(uint16_t lsn, uint16_t rsn, uint32_t pab)
	: m_protocolID(static_cast<uint32_t>(std::hash<std::string>()("Eros"))),
	m_id(lsn),
	m_ack(rsn),
	m_ackBitfieldInt(pab),
	m_isAcked(false),
	m_numMessages(0)
{
	m_remoteProtocolID = m_protocolID;
}

CPacket::CPacket()
	: m_isAcked(false)
{

}

template <typename Stream>
bool CPacket::SerializeInternal(Stream& stream)
{
	serialize_bits(stream, m_remoteProtocolID, 32);
	serialize_bits(stream, m_id, 16);
	serialize_bits(stream, m_ack, 16);
	serialize_bits(stream, m_ackBitfieldInt, 32);
	serialize_bits(stream, m_numMessages, 8);

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
