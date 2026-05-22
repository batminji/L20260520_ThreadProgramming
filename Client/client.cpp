#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WINDOWX 50
#define WINDOWY 50
#define WINDOWW 600
#define WINDOWH 600

#include "ChatPacket.h"
#include "NetUtil.h"

#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <conio.h>

#include "SDL.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "NetCommon")


using namespace std;

char SendBuffer[1024] = { 0, };
char RecvBuffer[1024] = { 0, };

bool IsRecvThreadRunning = true;
bool IsSendThreadRunning = true;

SessionManager MySessionManager;
SOCKET MyClientID;
int ClientDirection = ' ';
HANDLE KeyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
SDL_Renderer* Renderer;
SDL_Event Event;

void SDLRender(SDL_Renderer* Renderer, int InColorR, int InColorG, int InColorB, int InX, int InY)
{
	SDL_SetRenderDrawColor(Renderer, InColorR, InColorG, InColorB, 255);
	
	SDL_Rect Rect = { InX * 10, InY * 10, 30, 30 };
	SDL_RenderFillRect(Renderer, &Rect);
}

void Render()
{
	system("cls");
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_RenderClear(Renderer);
	COORD Where;

	for (auto Player : MySessionManager.SessionList)
	{
		Where.X = Player.X;
		Where.Y = Player.Y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Where);
		SDLRender(Renderer, Player.R, Player.G, Player.B, Where.X, Where.Y);
		SDL_RenderPresent(Renderer);
		std::cout << (char)Player.Shape << std::endl;
	}
}

void ProcessPacket(SOCKET ProcessSocket, const char* InBuffer, Header& InHeader)
{
	switch ((EPacketType)(InHeader.PacketType))
	{
	case EPacketType::SC_Login:
	{
		SC_Login LoginPacket;
		LoginPacket.Parse(InBuffer);

		// std::cout << LoginPacket.ToString() << std::endl;

		MyClientID = LoginPacket.ClientSocket;
	}
		break;
	case EPacketType::SC_Spawn:
	{
		SC_Spawn SpawnPacket;
		SpawnPacket.Parse(InBuffer);

		// std::cout << SpawnPacket.ToString() << std::endl;

		Session InSession;
		InSession.ClientSocket = SpawnPacket.ClientSocket;
		InSession.Shape = SpawnPacket.Shape;
		InSession.X = SpawnPacket.X;
		InSession.Y = SpawnPacket.Y;
		InSession.R = SpawnPacket.R;
		InSession.G = SpawnPacket.G;
		InSession.B = SpawnPacket.B;	

		MySessionManager.Add(InSession);
		Render();
	}
	break;
	case EPacketType::SC_Move:
	{
		SC_Move MovePacket;
		MovePacket.Parse(InBuffer);

		Session* FindSession = MySessionManager.GetSession(MovePacket.ClientSocket);
		FindSession->X = MovePacket.X;
		FindSession->Y = MovePacket.Y;

		// std::cout << MovePacket.ToString() << std::endl;
		Render();
	}
	break;
	case EPacketType::SC_Destroy:
	{
		SC_Destroy DestroyPacket;
		DestroyPacket.Parse(InBuffer);


		Session* FindSession = MySessionManager.GetSession(DestroyPacket.ClientSocket);

		// std::cout << "Quit : " << FindSession->ClientSocket << std::endl;
		MySessionManager.Delete(*FindSession);
		Render();
	}
	break;
	default:
	{

	}
		break;
	}

}

unsigned WINAPI RecvThread(void* Argument)
{
	SOCKET ServerSocket = *(SOCKET*)Argument;

	while (IsRecvThreadRunning)
	{
		Header DataHeader;
		int RecvBytes = RecvAll(ServerSocket, (char*)&DataHeader, HeaderSize);
		if (RecvBytes <= 0)
		{
			cout << "header recv fail " << endl;
			break;
		}
		DataHeader.NetworkToHost();

		memset(RecvBuffer, 0, sizeof(RecvBuffer));
		//data JSON
		RecvBytes = RecvAll(ServerSocket, RecvBuffer, DataHeader.PacketSize);
		if (RecvBytes <= 0)
		{
			cout << "recv fail " << endl;
			break;
		}

		ProcessPacket(ServerSocket, RecvBuffer, DataHeader);
	}


	return 0;
}

unsigned WINAPI SendThread(void* Argument)
{
	//책임은 사용하는 놈이 진다.
	SOCKET ServerSocket = *(SOCKET*)Argument;

	while (IsSendThreadRunning)
	{
		WaitForSingleObject(KeyEvent, INFINITE);

		Header DataHeader;
		CS_Move MoveData;
		MoveData.ClientSocket = MyClientID;
		MoveData.Direction = ClientDirection;
		DataHeader.MakeHeader((int)MoveData.ToString().length(), EPacketType::CS_Move);
		int SentBytes = SendAll(ServerSocket, (char*)&DataHeader, HeaderSize);
		if (SentBytes <= 0)
		{
			cout << "header send fail." << endl;
		}

		//Data
		SentBytes = SendAll(ServerSocket, MoveData.ToString().c_str(), (int)MoveData.ToString().length());
		if (SentBytes <= 0)
		{
			cout << "Data send fail." << endl;
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	// SDL Init
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow("SDL Engine", WINDOWX, WINDOWY, WINDOWW, WINDOWH, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	const Uint8* State = SDL_GetKeyboardState(NULL);

	// cout << "client" << endl;
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.0.95");
	ServerSockAddr.sin_port = htons(35000);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	cout << "client connect" << endl;

	Header LoginHeader;
	CS_Login LoginData;
	LoginData.UserID = "minji";
	LoginData.HashKey = "1aeffasdefdsj";

	LoginHeader.MakeHeader(static_cast<unsigned short>(LoginData.ToString().length()), EPacketType::CS_Login);
	SendAll(ServerSocket, (char*)&LoginHeader, HeaderSize);
	SendAll(ServerSocket, LoginData.ToString().c_str(), (int)LoginData.ToString().length());

	HANDLE ThreadHandles[2] = { 0, };

	//nonblocking, asynchrous
	ThreadHandles[0] = (HANDLE)_beginthreadex(0, 0, RecvThread, &ServerSocket, /*CREATE_SUSPENDED*/0, 0);
	ThreadHandles[1] = (HANDLE)_beginthreadex(0, 0, SendThread, &ServerSocket, /*CREATE_SUSPENDED*/0, 0);

	while (true)
	{
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_KEYDOWN)
			{
				if (State[SDL_SCANCODE_W])
				{
					ClientDirection = 'w';
					SetEvent(KeyEvent);
				}
				if (State[SDL_SCANCODE_S])
				{
					ClientDirection = 's';
					SetEvent(KeyEvent);
				}
				if (State[SDL_SCANCODE_A])
				{
					ClientDirection = 'a';
					SetEvent(KeyEvent);
				}
				if (State[SDL_SCANCODE_D])
				{
					ClientDirection = 'd';
					SetEvent(KeyEvent);
				}
			}
		}
	}

	//blocking
	WaitForMultipleObjects(2, ThreadHandles, FALSE, INFINITE);

	closesocket(ServerSocket);

	//TerminateThread(ThreadHandles[0], 0);
	//TerminateThread(ThreadHandles[1], 0);
	IsSendThreadRunning = false;
	IsRecvThreadRunning = false;


	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	CloseHandle(ThreadHandles[0]);
	CloseHandle(ThreadHandles[1]);

	WSACleanup();

	return 0;
}