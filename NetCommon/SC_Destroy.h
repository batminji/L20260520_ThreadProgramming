#pragma once
#include "Packet.h"
class SC_Destroy : public IPacket
{
public:
	SOCKET ClientSocket;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

