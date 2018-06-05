#pragma once

#include <chrono>
#include <deque>
#include <utility>
#include <vector>

#include "Comparators.h"
#include "Address.h"
#include "Packet.h"
#include "Socket.h"

#define STEADY_CLOCK_NOW std::chrono::steady_clock::now()
#define UINT16_HALF 32768

/* MAX_RTT: Maximum round-trip-time (in ms) before a packet is deemed lost */
#define MAX_RTT 1000

/* INVALID_RTT: A invalid round-trip-time (for initialization) */
#define INVALID_RTT -1

/* RTT_SHIFT_FACTOR: Connection round-trip-time's smoothing factor */
#define RTT_SMOOTH_FACTOR 0.1

/* MAX_PACKET_FLOW_RATE: Maximum acceptable connection RTT (ms) before a switch occurs from good->bad */
#define MAX_PACKET_FLOW_RATE 250

/* DEFAULT_PACKET_STABILIZATION_DELTA: Default time (s) required before the connection can switch from bad->good */
#define DEFAULT_PACKET_STABILIZATION_DELTA 5

/* MAX_PACKET_STABILZATION_DELTA: Maximum time (s) required before the connection can switch from bad->good */
#define MAX_PACKET_STABILZATION_DELTA 60

/* MIN_PACKET_STABILZATION_DELTA: Minimum time (s) required before the connection can switch from bad->good */
#define MIN_PACKET_STABILZATION_DELTA 1

/* MINIMUM_REQUIRED_PACKET_STABILIZATION_TIME: Minimum time (s) a connection needs to stay in 'good' to be considered stabilization */
#define MINIMUM_REQUIRED_PACKET_STABILIZATION_TIME 10

// -- BITFIELD SIZE
#define BITFIELD_SIZE 32


class CConnection
{
public:

	CConnection(CAddress& address, CSocket& socket);

	CAddress& GetAddress() const;

	void Update();

	bool Send(unsigned char payload[PAYLOAD_SIZE], bool SPL = false);
	bool Receive(unsigned char* buffer, size_t size);

private:

	// TODO: Consider changing to 32 bit val
	int64_t m_connectionRTT;

	CAddress& m_address;
	CSocket& m_socket;

	uint16_t m_localSequenceNum;
	uint16_t m_remoteSequenceNum;


	std::vector<CPacket> m_out;
	std::vector<CPacket> m_in;

	typedef GenericComparator<CPacket, uint16_t> SequenceComp;

	typedef std::chrono::steady_clock::time_point timePoint;
	timePoint	m_lastRecievedPacket;
	timePoint	m_lastSentPacket;

	std::ostringstream log;

	void LogPacketTransfer(const char* type, CPacket& packet);
	void LogQueueStatus(std::string& type, std::vector<CPacket>& d);

	void DetectPacketLoss();

	void DetectFlowChange();

	enum class EPacketFlowState
	{
		GOOD = 0,
		BAD
	};

	EPacketFlowState m_packetFlowState;

	timePoint m_lastThresholdPass;

	timePoint m_lastGoodToBadTransition;
	timePoint m_goodStateTracker;

	uint8_t m_packetStabilizationDelta;
	

	// Sequence comparitor
	inline bool SequenceGreaterThan(uint16_t s1, uint16_t s2)
	{
		return ((s1 > s2) && (s1 - s2 <= UINT16_HALF)) ||
			((s1 < s2) && (s2 - s1 > UINT16_HALF));
	}
};



