#pragma once
#include "Packet.h"
class SC_Move : public IPacket
{
public:
	SOCKET ClientSocket;
	int X;
	int Y;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

