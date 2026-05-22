#pragma once
#include "Packet.h"
class SC_Spawn : public IPacket
{
public:
	SOCKET ClientSocket;
	int X;
	int Y;
	int R;
	int G;
	int B;
	char Shape;

	void Parse(std::string InString) override;
	std::string ToString() override;
};

