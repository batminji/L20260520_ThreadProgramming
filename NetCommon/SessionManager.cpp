#include "pch.h"
#include "SessionManager.h"
#include <algorithm>

void SessionManager::Add(Session InSession)
{
	SessionList.push_back(InSession);
}

void SessionManager::Delete(Session InSession)
{
	SessionList.erase(std::find(SessionList.begin(), SessionList.end(), InSession));
}

Session* SessionManager::GetSession(int Index)
{
	return &SessionList[Index];
}

Session* SessionManager::GetSession(const SOCKET& InSocket)
{
	for (auto It = SessionList.begin(); It != SessionList.end(); ++It)
	{
		if ((*It).ClientSocket == InSocket)
		{
			return &(*It);
		}
	}
	return nullptr;
}

Session* SessionManager::GetSession(const Session& InSession)
{
	return &(*(std::find(SessionList.begin(), SessionList.end(), InSession)));
}
