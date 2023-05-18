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
#define SERVER_IP "127.0.0.1"
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
	WSADATA wsaData;//��ʼ��
	static sockaddr_in RecvAddr;         //��������ַ
	static sockaddr_in SenderAddr;       //���ص�ַ
	int localPort = LOCAL_PORT;         //���ؼ����˿�
	int Port = REMOTE_PORT;              //�����������˿�
	static int BufLen;
	static int l_naddLen1;

	/** �߳��˳���־���� */
	static bool sendExit;
	static bool revExit;

	/** �߳̾�� */
	volatile HANDLE    sendThread;
	volatile HANDLE    revThread;
};