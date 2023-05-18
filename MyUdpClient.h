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
	WSADATA wsaData;//��ʼ��
	sockaddr_in RecvAddr;         //��������ַ
	sockaddr_in SenderAddr;       //���ص�ַ
	int localPort = LOCAL_PORT;         //���ؼ����˿�
	int Port = SERVER_PORT;              //�����������˿�
	int BufLen;
	int l_naddLen1;

	/** �߳��˳���־���� */
	static bool sendExit;
	static bool revExit;

	/** �߳̾�� */
	volatile HANDLE    sendThread;
	volatile HANDLE    revThread;

};