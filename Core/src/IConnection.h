#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include "Address.h"
#include "Socket.h"
#include "ChilliException.h"
#include "Packet.h"
#include <Message\SimpleMessage\SimpleMessage.h>


class IChannel
{
public:
	IChannel(unsigned short port) : m_port(port) {}
	virtual ~IChannel() { }

	virtual void Update() = 0;

	virtual void Open() = 0;
	virtual void Close()= 0;

	virtual void Receive(CAddress& address) = 0;

protected:
	unsigned short m_port;
};

class ReliableChannel : IChannel
{
public:
	ReliableChannel(unsigned short port)
		: IChannel(port)
	{
	}

	virtual ~ReliableChannel() override
	{
		// Close socket on deallocation
		Close();
	}

	virtual void Open() override
	{
		m_socket.Open(m_port);
	}

	virtual void Close() override
	{
		if (m_socket.IsOpen())
		{
			m_socket.Close();
		}
	}

	virtual void Update() override
	{
	}

	virtual void Receive(CAddress& address) override
	{
		unsigned char* tempBuf[PACKET_SIZE];
		int bytesRead = m_socket.Receive(address, tempBuf, PACKET_SIZE);

		CPacket packet;
		memcpy(packet.GetBuffer(), tempBuf, PACKET_SIZE);
		CReadStream stream(tempBuf, PACKET_SIZE);
		packet.Serialize(stream);
	}

private:
	CSocket m_socket;
};

struct IConnection
{
public:

	enum class ChannelType
	{
		RELIABLE,
		UNRELIABLE
	};

	/// Creates a new connection with this channel's address
	virtual std::shared_ptr<IChannel> CreateChannel(unsigned short port, ChannelType type) = 0;
	virtual void Update() = 0;

	virtual ~IConnection() { }
	IConnection() { }

	IConnection(const IConnection&) = delete;
	IConnection& operator=(const IConnection&) = delete;

	void Recieve(CSocket& socket)
	{
		CAddress sender;
		int bytesRead = socket.Receive(sender, m_receiveBuffer, PACKET_SIZE);

		UDP_TRAP(sender == m_remoteAddress); // TODO: Remove when we have tested this functionality


		// We only process this packet if:
		//	1) The sender is associated with this connection
		//  2) A positive number of bytes where read from the socket
		if (sender == m_remoteAddress && bytesRead > 0)
		{
			CReadStream stream(m_receiveBuffer, PACKET_SIZE);
			ReadContents(stream);
		}

		// Clear buffer
		memset(m_receiveBuffer, '\0', PACKET_SIZE);
	}

	void ReadContents(CReadStream& stream)
	{
		CPacket packet;
		packet.Serialize(stream);

		// TODO: For a default constructed packet, we need to initialize the potocol id.
		/*if (!packet.IsValidProtocolID())
		{

		}*/

		std::vector<std::shared_ptr<CMessage>> messages;
		CSimpleMsgFactory facRecv;

		ExtractMessages(stream, packet, facRecv, messages);
		
	}

	template <typename Stream>
	void ExtractMessages(
		Stream& stream,
		CPacket& packet,
		CMessageFactory& factory,
		std::vector<std::shared_ptr<CMessage>>& messages)
	{

		if (Stream::IsReading)
		{
			for (int i = 0; i < packet.GetNumMessages(); i++)
			{
				// Extract the message type
				uint16_t messageType;
				serialize_bits(stream, messageType, 0, 16);

				if (auto msgRecvOptional =
					factory.CreateMessage(messageType))
				{
					std::shared_ptr<CMessage> msg = msgRecvOptional.value();
					msg->Serialize(stream);
					messages.push_back(msg);
				}
			}
		}
	}

	void ExtractPayload()
	{

	}

protected:
	CAddress m_remoteAddress;

private:
	unsigned char m_receiveBuffer[PACKET_SIZE];
};

class BasicConnection : IConnection
{
public:

	BasicConnection(CAddress address)
	{
		m_remoteAddress = address;
	}

	virtual std::shared_ptr<IChannel> CreateChannel(unsigned short port, ChannelType type) override
	{
		std::shared_ptr<IChannel> channel;

		switch (type)
		{
		case ChannelType::RELIABLE:
		{
			channel = std::dynamic_pointer_cast<IChannel>(std::make_shared<ReliableChannel>(port));
		}
		default:
			throw ChiliException(__LINE__, __FILE__);
		}

		m_channels.push_back(channel);
		return channel;
	}

	template <typename T>
	std::shared_ptr<T> CreateChannel(unsigned short port)
	{
		static_assert(std::is_base_of<IChannel, T>::value, "Channel must derive from IChannel");
		
		std::shared_ptr<T> channel = std::make_shared<T>(port);
		m_channels.push_back(channel);
		return channel;
	}

	virtual void Update() override
	{
		// Update each channel
		for (auto channel : m_channels)
		{
			// Check if channel receives data over socket from m_address
			channel->Receive(m_remoteAddress);
		}
	}
	
private:
	std::vector<std::shared_ptr<IChannel>> m_channels;
};


void useCase()
{
	// From a client perspective:
	CAddress serverAddress(227, 4, 1, 1, 2000);

	BasicConnection serverConnection(serverAddress);

	// Create a new channel
	std::shared_ptr<ReliableChannel> myChannel = serverConnection.CreateChannel<ReliableChannel>(4002);

	myChannel->Open();

	while (true)
	{
		serverConnection.Update();

	}
}