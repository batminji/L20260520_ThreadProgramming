#include "pch.h"
#include "CS_Login.h"

void CS_Login::Parse(std::string InString)
{
    JSONDocument.Parse(InString.c_str());

    UserID = JSONDocument["UserID"].GetString();
    HashKey = JSONDocument["HashKey"].GetString();
}

std::string CS_Login::ToString()
{
    JSONDocument.SetObject();
    JSONDocument.AddMember("UserID", UserID, JSONDocument.GetAllocator());
    JSONDocument.AddMember("HashKey", HashKey, JSONDocument.GetAllocator());

    rapidjson::StringBuffer Buffer;
    rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
    JSONDocument.Accept(Writer);

    return Buffer.GetString();
}
