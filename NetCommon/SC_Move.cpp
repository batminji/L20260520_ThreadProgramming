#include "pch.h"
#include "SC_Move.h"

void SC_Move::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    ClientSocket = JSONDocument["ClientSocket"].GetInt();
    X = JSONDocument["X"].GetInt();
    Y = JSONDocument["Y"].GetInt();
}

std::string SC_Move::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("ClientSocket", ClientSocket, JSONDocument.GetAllocator());
    JSONDocument.AddMember("X", X, JSONDocument.GetAllocator());
    JSONDocument.AddMember("Y", Y, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
