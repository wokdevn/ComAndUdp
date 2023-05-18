#include "MyUdpClient.h"

/** 线程退出标志 */
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
		perror("发送失败");
		exit(1);
	}

	printf("\nSend：");
	for (int i = 0; i < strlen(SendBuf); i++)
	{
		printf("%02x ", (UCHAR)SendBuf[i]);
	}
}

bool MyUdpClient::OpenSendThread()
{
	/** 检测线程是否已经开启了 */
	if (sendThread != INVALID_HANDLE_VALUE)
	{
		/** 线程已经开启 */
		std::cout << "Send thread already open\n\n";
		return false;
	}

	sendExit = false;
	/** 线程ID */
	UINT threadId;
	/** 开启串口数据监听线程 */
	sendThread = (HANDLE)_beginthreadex(NULL, 0, SendThreadFunc, this, 0, &threadId);
	if (!sendThread)
	{
		return false;
	}
	/** 设置线程的优先级,高于普通线程 */
	if (!SetThreadPriority(sendThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

UINT WINAPI MyUdpClient::SendThreadFunc(void* pParam) {
	char SendBuf[SENDBUFFSIZE];
	int l_nReadLen = recvfrom(SendSocket, SendBuf, BufLen, 0, (struct sockaddr*)&SenderAddr, &l_naddLen1);
	printf("\nread：");
	for (int i = 0; i < l_nReadLen; i++)
	{
		printf("%02x ", SendBuf[i]);
	}

	return 0;
}

bool MyUdpClient::CloseRevThread() {
	if (revThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		revExit = true;

		/** 等待线程退出 */
		Sleep(10);

		/** 置线程句柄无效 */
		CloseHandle(revThread);
		revThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool MyUdpClient::CloseSendThread() {
	if (sendThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		sendExit = true;

		/** 等待线程退出 */
		Sleep(10);

		/** 置线程句柄无效 */
		CloseHandle(sendThread);
		sendThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

MyUdpClient::MyUdpClient(void) {
	init = WSAStartup(MAKEWORD(2, 2), &wsaData);

	localPort = LOCAL_PORT;         //本地监听端口
	Port = SERVER_PORT;				//服务器监听端口

	BufLen = 1024;

	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//设置服务器地址
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
