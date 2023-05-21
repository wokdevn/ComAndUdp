/*udp������*/

#include <stdio.h>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <process.h>
#include <iostream>

#include "Command.h"
#include "Util.h" 

//Ԥ����ָ��,�����������ָ�����ӿ�
#pragma comment(lib,"ws2_32.lib")

//��������
#define REMOTE_PORT 8001
#define LOCAL_PORT  8000
#define REMOTE_IP "127.0.0.1"
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

	//_stdcall�ķ���,����_beginthreadex����,���ܷ�������,����ö����static��
	static UINT WINAPI RevThreadFunc(void* pParam);
	static UINT WINAPI SendThreadFunc(void* pParam);

	SOCKET NetSocket;
	WSADATA wsaData;					//���������׽���,��Ҫ�ǵ���������̬���ӿ�
	sockaddr_in RemtAddr;
	sockaddr_in LoclAddr;
	int localPort = LOCAL_PORT;
	int remotePort = REMOTE_PORT;
	int BufLen;
	int l_naddLen1;

	/*�߳��˳���־*/
	static bool sendExit;
	static bool revExit;

	/*�߳̾��*/
	volatile HANDLE sendThread;
	volatile HANDLE revThread;
};
