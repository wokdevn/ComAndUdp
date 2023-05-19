#include <stdio.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <iostream>
#include "Command.h"
#include "Util.h" 

#pragma comment(lib,"ws2_32.lib")

#define REMOTE_PORT 8001
#define LOCAL_PORT  8000
#define SERVER_IP "192.168.0.27"
#define SENDBUFFSIZE 1024
#define REVBUFFSIZE 1024

using namespace std;

class MyUdpClient
{
public:
	MyUdpClient();
	~MyUdpClient();

public:
	bool OpenRevThread();
	static UINT WINAPI RevThreadFunc(void* pParam);
	bool OpenSendThread();
	static UINT WINAPI SendThreadFunc(void* pParam);
	bool CloseRevThread();
	bool CloseSendThread();
	void StartThread();
	int SendPack();

	int init;
	static SOCKET SendSocket;
	WSADATA wsaData;//初始化
	static sockaddr_in RecvAddr;         //服务器地址
	static sockaddr_in SenderAddr;       //本地地址
	int localPort = LOCAL_PORT;         //本地监听端口
	int Port = REMOTE_PORT;              //服务器监听端口
	static int BufLen;
	static int l_naddLen1;

	/** 线程退出标志变量 */
	static bool sendExit;
	static bool revExit;

	/** 线程句柄 */
	volatile HANDLE    sendThread;
	volatile HANDLE    revThread;
};