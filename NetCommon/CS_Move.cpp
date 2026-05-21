#include "pch.h"
#include "CS_Move.h"

void CS_Move::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    ClientSocket = JSONDocument["ClientSocket"].GetInt();
    Direction = JSONDocument["Direction"].GetString();
}

std::string CS_Move::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("ClientSocket", ClientSocket, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Direction", Direction, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
