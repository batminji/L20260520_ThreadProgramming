#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetUtil.h"

#include <winsock2.h>
#include <iostream>


#pragma comment(lib, "ws2_32")
#pragma comment(lib, "NetCommon")

using namespace std;

char Buffer[1024] = { 0, };

void ProcessPacket(SOCKET ProcessSocket, const char* InBuffer, Header& InHeader)
{
	switch ((EPacketType)InHeader.PacketType)
	{
	case EPacketType::CS_Login:
		CS_Login LoginPacket;
		LoginPacket.Parse(InBuffer);
		
		break;
	case EPacketType::CS_Move:
		break;
	default:
		break;
	}

	// //header
	// int SentBytes = SendAll(ReadSockets.fd_array[Index], (char*)&PacketSize, 2);
	// if (SentBytes <= 0)
	// {
	// 	cout << "header send fail." << endl;
	// 	DisconnectSocket(ReadSockets.fd_array[Index], &ReadSockets);
	// }
	// 
	// //Data
	// SentBytes = SendAll(ReadSockets.fd_array[Index], Buffer, ntohs(PacketSize));
	// if (SentBytes <= 0)
	// {
	// 	cout << "Data send fail." << endl;
	// 	DisconnectSocket(ReadSockets.fd_array[Index], &ReadSockets);
	// }
}

//blocking, synchrous, multiplexing(polling)
int main()
{
	cout << "server start" << endl;

	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(35000);

	//already use port 이미 포트 사용중
	::bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, SOMAXCONN);



	//blocking, synchronous(TimeOut)
	TIMEVAL TimeOut;
	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 500000;

	fd_set ReadSockets;
	fd_set CopyReadSockets;

	FD_ZERO(&ReadSockets);
	FD_SET(ListenSocket, &ReadSockets);

	while (true)
	{
		CopyReadSockets = ReadSockets;

		//0.5초씩 blocking
		int ChangeCount = select(0, &CopyReadSockets, 0, 0, &TimeOut);

		if (ChangeCount <= 0)
		{
			//Server Work
			//0.5초한번 서버 작업을 하는거
			continue;
		}

		//몬가 자료 있다.
		for (int i = 0; i < (int)ReadSockets.fd_count; ++i)
		{
			if (FD_ISSET(ReadSockets.fd_array[i], &CopyReadSockets))
			{
				if (ReadSockets.fd_array[i] == ListenSocket)
				{
					//connect process
					SOCKADDR_IN ClientSockAddr;
					memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
					int ClientSockSockLength = sizeof(ClientSockAddr);

					//blocking, synchronous
					SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockSockLength);

					cout << "connect client " << inet_ntoa(ClientSockAddr.sin_addr) << endl;

					FD_SET(ClientSocket, &ReadSockets);
				}
				else
				{
					//Data Receive

					//header
					Header DataHeader;
					int RecvBytes = RecvAll(ReadSockets.fd_array[i], (char*)&DataHeader, HeaderSize);
					if (RecvBytes <= 0)
					{
						cout << "header recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}

					DataHeader.NetworkToHost();

					memset(Buffer, 0, sizeof(Buffer));
					//data JSON
					RecvBytes = RecvAll(ReadSockets.fd_array[i], Buffer, DataHeader.PacketSize);
					if (RecvBytes <= 0)
					{
						cout << "data recv fail " << endl;
						DisconnectSocket(ReadSockets.fd_array[i], &ReadSockets);
						continue;
					}
					else
					{
						ProcessPacket(ReadSockets.fd_array[i], Buffer, DataHeader);
					}
				}
			}
		}
	}






	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}