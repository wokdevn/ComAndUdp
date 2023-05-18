#include <stdio.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT 4000
#define LOCAL_PORT  4001
#define SERVER_IP "127.0.0.1"
#define SENDBUFFSIZE 1024

using namespace std;

class MyUdpClient
{
public:
	MyUdpClient(void);
	~MyUdpClient(void);

	bool OpenRevThread();
	static UINT WINAPI RevThreadFunc(void* pParam);
	bool OpenSendThread();
	static UINT WINAPI SendThreadFunc(void* pParam);
	bool CloseRevThread();
	bool CloseSendThread();

	int init;
	SOCKET SendSocket;
	WSADATA wsaData;//初始化
	sockaddr_in RecvAddr;         //服务器地址
	sockaddr_in SenderAddr;       //本地地址
	int localPort = LOCAL_PORT;         //本地监听端口
	int Port = SERVER_PORT;              //服务器监听端口
	int BufLen;
	int l_naddLen1;

	/** 线程退出标志变量 */
	static bool sendExit;
	static bool revExit;

	/** 线程句柄 */
	volatile HANDLE    sendThread;
	volatile HANDLE    revThread;

};