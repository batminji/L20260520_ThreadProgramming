#pragma once
#include "Packet.h"
class CS_Login : public IPacket
{
public:
	void Parse(std::string InString) override;
	std::string ToString() override;
};

