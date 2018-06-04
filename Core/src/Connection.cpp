#include "Connection.h"
#include <algorithm>
#include "LogManager.h"

CConnection::CConnection(CAddress& address, CSocket& socket)
	: m_address(address),
	m_socket(socket),
	m_localSequenceNum(0),
	m_remoteSequenceNum(0),
	m_lastRecievedPacket(STEADY_CLOCK_NOW),
	m_lastSentPacket(STEADY_CLOCK_NOW),
	m_connectionRTT(INVALID_RTT),
	m_packetFlowState(EPacketFlowState::GOOD),
	m_lastThresholdPass(STEADY_CLOCK_NOW),
	m_lastGoodToBadTransition(STEADY_CLOCK_NOW),
	m_goodStateTracker(STEADY_CLOCK_NOW),
	m_packetStabilizationDelta(DEFAULT_PACKET_STABILIZATION_DELTA)
{
	// Log message
	log << "{New Connection} (" << address.GetFormattedAddress() << ":" << address.GetPort() << ")";
	CLogManager::Instance().WriteLine(log.str());
	log.str("");

}


CAddress& CConnection::GetAddress() const
{
	return m_address;
}


// TODO: Possibly remove
void CConnection::ReceivePacket(CPacket& packet)
{

	// Add packet to IN-QUEUE
	m_in.push_back(packet);

	uint16_t packetSequenceID = packet.GetID();
	
	// If packet sequence ID  >  remote sequence number
	if (SequenceGreaterThan(packetSequenceID, m_remoteSequenceNum))
	{
		// Update the remote sequence number
		m_remoteSequenceNum = packetSequenceID;
	}

	// Get ACK
	uint16_t ack = packet.GetAck();

	// Get ACK bitfield
	uint32_t ackBitfieldInt = packet.GetAckBitfieldInt();
	std::bitset<BITFIELD_SIZE> ackBitfield(ackBitfieldInt);

	// Log message
	LogPacketTransfer("Recieved Packet", packet);

	// Loop each ack bit
	for (uint16_t i = 0; i < BITFIELD_SIZE; i++)
	{
		// If bit is acked
		if (ackBitfield[i])
		{
			// Get sequence number to ack
			uint16_t sequenceNumber = ack - i;

			// Search for sequence number in OUT-QUEUE 
			auto it = std::find_if(m_out.begin(), m_out.end(), SequenceComp(&CPacket::GetID, sequenceNumber));

			// Packet found
			if (it != m_out.end())
			{
				if (it->IsAcked()) continue;

				log << "{Acked Packet} (" << m_address.GetFormattedAddress() << ":" << m_address.GetPort() << ")" << " (PS: " << it->GetID() << ")";
				CLogManager::Instance().WriteLine(log.str());
				log.str("");
				

				// Ack the packet
				it->AckPacket();

			
				{
					using namespace std::chrono;
					auto now = steady_clock::now();

					uint64_t rtt = duration_cast<milliseconds> (now - it->GetSentTime()).count();
					
					// If the connection RTT is currently invalid
					if (m_connectionRTT <= INVALID_RTT)
					{
						// Initialize it to this packet's RTT
						m_connectionRTT = rtt;
					}
					else
					{
						int64_t diff = rtt - m_connectionRTT;
						m_connectionRTT += (int64_t) diff*RTT_SMOOTH_FACTOR;

					}


					// Log RTT
					log << "RTT: " << rtt << "ms";
					CLogManager::Instance().WriteLine(log.str());
					log.str("");

					log << "Connection RTT: " << m_connectionRTT;
					CLogManager::Instance().WriteLine(log.str());
					log.str("");

				}
				
				// Erase packet from outgoing queue
				m_out.erase(it);

			}

		}

	}

	LogQueueStatus(std::string("Out-Queue"), m_out);
	LogQueueStatus(std::string("In-Queue"), m_in);

}

// Send payload to this connection. 
// NOTE: SPL (simulated packet loss) SHOULD ONLY BE SET FOR TESTING PURPOSES (duh...)
bool CConnection::Send(unsigned char payload[PAYLOAD_SIZE], bool SPL /* = false */)
{

	// Increment local sequence number
	m_localSequenceNum++;

	std::bitset<BITFIELD_SIZE> ackBitfield;

	// Loop each ack bit
	for (uint16_t i = 0; i < BITFIELD_SIZE; i++)
	{
		
		// Get sequence number to check ack
		uint16_t sequenceNumber = m_remoteSequenceNum - i;

		// Search for sequence number in IN-QUEUE 
		auto it = std::find_if(m_in.begin(), m_in.end(), SequenceComp(&CPacket::GetID, sequenceNumber));
		
		// Packet found
		if (it != m_in.end())
		{
			// Acknowledge packet "sequenceNumber" has been recieved
			ackBitfield.set(i);
		} 
		
	}

	// Convert bitfield to bitfield-integer
	uint32_t ackBitfieldInt = static_cast<uint32_t>(ackBitfield.to_ulong());

	// Create a new packet
	CPacket packet(m_localSequenceNum, m_remoteSequenceNum, ackBitfieldInt);
	
	// Construct packet
	if (!packet.ConstructPacket(payload))
	{
		// if error occurs while constructing
		return false;
	}


	// Set the packet sent time to NOW
	{
		using namespace std::chrono;
		packet.SetSentTime(steady_clock::now());

	}

	// Send packet (NOTE: SPL flag!!)
	if (!SPL && !m_socket.Send(m_address, packet.GetBuffer(), PACKET_SIZE))
	{
		// Error while sending
		return false;
	}
	
	// Push packet to out queue
	m_out.push_back(packet);
	
	
	std::string sendMessage;

	if (SPL)
	{
		sendMessage = "SPL Send Packet";
	}
	else
	{
		sendMessage = "Send Packet";
	}

	// Log message
	LogPacketTransfer(sendMessage.c_str(), packet);

	LogQueueStatus(std::string("Out-Queue"), m_out);
	LogQueueStatus(std::string("In-Queue"), m_in);

	return true;
}

void CConnection::DetectPacketLoss()
{

	// Loop out queue
	auto it = m_out.begin();

	while (it != m_out.end())
	{

		// if packet is acked, don't bother
		if (!it->IsAcked())
		{

			using namespace std::chrono;
			auto now = steady_clock::now();
			uint64_t duration = duration_cast<milliseconds> (now - it->GetSentTime()).count();

			// if RTT of packet is greater than maximum
			if (duration > MAX_RTT)
			{

				LogPacketTransfer("Dropped Packet", *it);
	
				// remove the packet from the out queue
				it = m_out.erase(it);
				continue;
			}

		}

		++it;
	}

}

void CConnection::DetectFlowChange()
{

	// Get now time
	auto now = STEADY_CLOCK_NOW;

	// CASE: connection RTT is beyond maximum flow rate
	if (m_connectionRTT > MAX_PACKET_FLOW_RATE)
	{
		// Reset last threshold pass
		m_lastThresholdPass = STEADY_CLOCK_NOW;
	
		// STATE TRANSITION:									-- GOOD->BAD --
		if (m_packetFlowState == EPacketFlowState::GOOD)
		{
		
			m_packetFlowState = EPacketFlowState::BAD;
			auto durationInGood = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastGoodToBadTransition).count();

			// If the time in GOOD <= 10s
			if (durationInGood <= MINIMUM_REQUIRED_PACKET_STABILIZATION_TIME)
			{
				
				uint8_t val = m_packetStabilizationDelta * 2;

				if (val > MAX_PACKET_STABILZATION_DELTA) m_packetStabilizationDelta = MAX_PACKET_STABILZATION_DELTA;
				else									 m_packetStabilizationDelta = val;

			}

			m_lastGoodToBadTransition = STEADY_CLOCK_NOW;
			m_goodStateTracker = STEADY_CLOCK_NOW;
		}
	}
	
	// CASE: Current flow state = BAD
	else if (m_packetFlowState == EPacketFlowState::BAD)
	{
		auto durationStabilized = std::chrono::duration_cast<std::chrono::seconds> (now - m_lastThresholdPass).count();

		// STATE TRANSITION:									-- BAD->GOOD --
		if (durationStabilized >= m_packetStabilizationDelta)
		{
			m_packetFlowState = EPacketFlowState::GOOD;
		}

	}

	// CASE: Current flow state = GOOD
	else if (m_packetFlowState == EPacketFlowState::GOOD)
	{
		auto duration = std::chrono::duration_cast<std::chrono::seconds> (now - m_goodStateTracker).count();

		// every 10 seconds while in GOOD, reduce the stabilization delta
		if (duration >= MINIMUM_REQUIRED_PACKET_STABILIZATION_TIME)
		{
			uint8_t val = m_packetStabilizationDelta / 2;

			if (val < MIN_PACKET_STABILZATION_DELTA) m_packetStabilizationDelta = MIN_PACKET_STABILZATION_DELTA;
			else m_packetStabilizationDelta = val;

			m_goodStateTracker = now;
		}

	}

}


void CConnection::Update()
{
	DetectPacketLoss();

	DetectFlowChange();

	auto now = STEADY_CLOCK_NOW;

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (now - m_lastSentPacket).count();

	if (duration > 1000)
	{
		std::cout << m_packetStabilizationDelta << std::endl;

		//std::cout << "send packet" << std::endl;
		m_lastSentPacket = std::chrono::steady_clock::now();

		//std::cout << m_in.front().GetID() << std::endl;


		//std::cout << "Timeout!" << std::endl;
	}

}

void CConnection::LogPacketTransfer(const char* type, CPacket& packet)
{
	std::ostringstream log;
	std::bitset<BITFIELD_SIZE> ackBitfield(packet.GetAckBitfieldInt());
	log << "{" << type << "} (" << m_address.GetFormattedAddress() << ":" << m_address.GetPort() << ")" << " (PS: " << packet.GetID() << ", ACK: "
		<< packet.GetAck() << ", PAB: " << ackBitfield << " (MSG = " << packet.GetPayload() << ")";
	CLogManager::Instance().WriteLine(log.str());
}

void CConnection::LogQueueStatus(std::string& type, std::vector<CPacket>& d)
{
	std::ostringstream log;

	log << type << ": ";

	for (auto it = d.begin(); it != d.end(); ++it)
	{
		CPacket& packet = *it;
		bool acked = packet.IsAcked();

		if (acked)
		{
			log  << "(";
		}

		log << packet.GetID();

		if (acked)
		{
			log << ")";
		}

		log << " | ";

		
	}

	std::cout << log.str() << std::endl;

	CLogManager::Instance().WriteLine(log.str());

}

