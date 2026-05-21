#pragma once
#include "Packet.h"
class CS_Move : public IPacket
{
public:
	SOCKET ClientSocket;
	std::string Direction;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

