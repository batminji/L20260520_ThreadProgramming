#include "pch.h"
#include "SC_Destroy.h"

void SC_Destroy::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

	ClientSocket = JSONDocument["ClientSocket"].GetInt();
}

std::string SC_Destroy::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("ClientSocket", ClientSocket, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
