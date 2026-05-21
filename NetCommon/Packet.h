#pragma once
#include "pch.h"

class IPacket
{
public:
	virtual void Parse(std::string InString) = 0;
	virtual std::string ToString() = 0;
	virtual int Length() {};

	rapidjson::Document JSONDocument;
};

