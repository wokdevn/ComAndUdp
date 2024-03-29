/*udp控制类*/

#include <stdio.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <iostream>

#include "Command.h"
#include "Util.h" 

//预处理指令,这里的作用是指定链接库
#pragma comment(lib,"ws2_32.lib")

//参数设置
#define REMOTE_PORT 8001
#define LOCAL_PORT  8000
#define REMOTE_IP "192.168.0.27"
#define SENDBUFFSIZE 1024
#define REVBUFFSIZE 1024
#define SLEEP_TIME_UDP 1

using namespace std;

class MyUdpClient
{
public:
	MyUdpClient();
	~MyUdpClient();

public:
	bool OpenRevThread();
	bool OpenSendThread();
	bool CloseRevThread();
	bool CloseSendThread();

	void StartThreadTxRx();
	int  SendPack();

	//_stdcall的方法,用于_beginthreadex调用,不能放在类中,否则得定义成static的
	static UINT WINAPI RevThreadFunc(void* pParam);
	static UINT WINAPI SendThreadFunc(void* pParam);

	SOCKET NetSocket;
	WSADATA wsaData;					//用于启动套接字,主要是调用起来动态链接库
	sockaddr_in RemtAddr;
	sockaddr_in LoclAddr;
	int localPort = LOCAL_PORT;
	int remotePort = REMOTE_PORT;
	int BufLen;
	int l_naddLen1;

	/*线程退出标志*/
	static bool sendExit;
	static bool revExit;

	/*线程句柄*/
	volatile HANDLE sendThread;
	volatile HANDLE revThread;
};
