#pragma once
#include "pch.h"
#include <vector>
#include <set>

struct Session
{
	SOCKET ClientSocket;
	std::string UserID;

	int X;
	int Y;

	char Shape = ' ';

	bool operator==(const Session& RHS)
	{
		return this->ClientSocket == RHS.ClientSocket;
	}
};

class SessionManager
{
public:
	void Add(Session InSession);
	void Delete(Session InSession);

	Session* GetSession(int Index);
	Session* GetSession(const SOCKET& InSocket);
	Session* GetSession(const Session& InSession);

	std::vector<Session> SessionList;
};

