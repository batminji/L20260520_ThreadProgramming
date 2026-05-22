#pragma once

#include "pch.h"
#include "ChatPacket.h"
#include "CS_Login.h"
#include "CS_Move.h"
#include "SC_Destroy.h"
#include "SC_Login.h"
#include "SC_Move.h"
#include "SC_Spawn.h"

#include "SessionManager.h"

enum class EPacketType : unsigned short
{
	// Client To Server
	CS_Login,
	CS_Move,

	// Server To Client
	SC_Login,
	SC_Move,
	SC_Destroy,
	SC_Spawn,

	ChatPacket,
	Max
};
#pragma pack(push, 1)

struct Header
{
	unsigned short PacketSize;
	unsigned short PacketType;

	void MakeHeader(int InPackerSize, EPacketType InPacketType)
	{
		PacketSize = htons(InPackerSize);
		PacketType = htons(static_cast<unsigned short>(InPacketType));
	}

	void NetworkToHost()
	{
		PacketSize = ntohs(PacketSize);
		PacketType = ntohs(PacketType);
	}
};

#pragma pack(pop)

const unsigned short HeaderSize = sizeof(Header);

extern int RecvAll(SOCKET ReceiverSocket, char* OutData, int Size);

extern int SendAll(SOCKET ReceiverSocket, const char* InData, int Size);