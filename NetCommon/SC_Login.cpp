#include "pch.h"
#include "SC_Login.h"

void SC_Login::Parse(std::string InString)
{
	JSONDocument.Parse(InString.c_str());

    ClientSocket = JSONDocument["ClientSocket"].GetInt();
    Message = JSONDocument["Message"].GetString();
}

std::string SC_Login::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("ClientSocket", ClientSocket, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Message", Message, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
