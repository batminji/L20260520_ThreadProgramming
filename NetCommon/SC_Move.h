#pragma once
#include "Packet.h"
class SC_Move : public IPacket
{
public:
	void Parse(std::string InString) override;
	std::string ToString() override;
};

