#pragma once
#include "Packet.h"
class SC_Login : public IPacket
{
public:
	SOCKET ClientSocket;
	std::string Message;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

