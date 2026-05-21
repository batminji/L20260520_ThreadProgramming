#pragma once
#include "Packet.h"
class CS_Login : public IPacket
{
public:
	std::string UserID;
	std::string HashKey;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

