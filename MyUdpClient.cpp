#include "MyUdpClient.h"

/** �߳��˳���־ */
bool MyUdpClient::sendExit = false;
bool MyUdpClient::revExit = false;


bool MyUdpClient::OpenRevThread() {
	if (revThread != INVALID_HANDLE_VALUE) {
		std::cout << "Rev thread already open\n\n";
		return false;
	}

	revExit = false;

	UINT threadId;

	revThread = (HANDLE)_beginthreadex(NULL, 0, RevThreadFunc, this, 0, &threadId);

	if (!revThread) {
		return false;
	}

	if (!SetThreadPriority(revThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

UINT WINAPI MyUdpClient::RevThreadFunc(void* pParam) {
	char SendBuf[SENDBUFFSIZE];
	strcpy_s(SendBuf, "hello");
	int l_nLen = sendto(SendSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (l_nLen < 0)
	{
		perror("����ʧ��");
		exit(1);
	}

	printf("\nSend��");
	for (int i = 0; i < strlen(SendBuf); i++)
	{
		printf("%02x ", (UCHAR)SendBuf[i]);
	}
}

bool MyUdpClient::OpenSendThread()
{
	/** ����߳��Ƿ��Ѿ������� */
	if (sendThread != INVALID_HANDLE_VALUE)
	{
		/** �߳��Ѿ����� */
		std::cout << "Send thread already open\n\n";
		return false;
	}

	sendExit = false;
	/** �߳�ID */
	UINT threadId;
	/** �����������ݼ����߳� */
	sendThread = (HANDLE)_beginthreadex(NULL, 0, SendThreadFunc, this, 0, &threadId);
	if (!sendThread)
	{
		return false;
	}
	/** �����̵߳����ȼ�,������ͨ�߳� */
	if (!SetThreadPriority(sendThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

UINT WINAPI MyUdpClient::SendThreadFunc(void* pParam) {
	char SendBuf[SENDBUFFSIZE];
	int l_nReadLen = recvfrom(SendSocket, SendBuf, BufLen, 0, (struct sockaddr*)&SenderAddr, &l_naddLen1);
	printf("\nread��");
	for (int i = 0; i < l_nReadLen; i++)
	{
		printf("%02x ", SendBuf[i]);
	}

	return 0;
}

bool MyUdpClient::CloseRevThread() {
	if (revThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */
		revExit = true;

		/** �ȴ��߳��˳� */
		Sleep(10);

		/** ���߳̾����Ч */
		CloseHandle(revThread);
		revThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool MyUdpClient::CloseSendThread() {
	if (sendThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */
		sendExit = true;

		/** �ȴ��߳��˳� */
		Sleep(10);

		/** ���߳̾����Ч */
		CloseHandle(sendThread);
		sendThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

MyUdpClient::MyUdpClient(void) {
	init = WSAStartup(MAKEWORD(2, 2), &wsaData);

	localPort = LOCAL_PORT;         //���ؼ����˿�
	Port = SERVER_PORT;				//�����������˿�

	BufLen = 1024;

	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//���÷�������ַ
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	inet_pton(AF_INET, SERVER_IP, &RecvAddr.sin_addr);

	SenderAddr.sin_family = AF_INET;
	SenderAddr.sin_port = htons(localPort);
	SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(SendSocket, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));

	l_naddLen1 = sizeof(SenderAddr);

	sendThread = INVALID_HANDLE_VALUE;
	revThread = INVALID_HANDLE_VALUE;
}

MyUdpClient::~MyUdpClient(void) {
	closesocket(SendSocket);
	WSACleanup();
	CloseRevThread();
	CloseSendThread();
}
