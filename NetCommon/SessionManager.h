#pragma once
#include "pch.h"
#include <vector>

struct Session
{
	SOCKET ClientSocket;
	std::string UserID;

	int X;
	int Y;
};

class SessionManager
{
public:
	void Add(Session InSession);
	void Delete(Session InSesstion);

	Session& GetSession(int Index);
	Session& GetSession(const SOCKET& InSocket);

	std::vector<Session> SessionList;
};

