#include "SerialPort.h"

CSerialPort* MyUdpClient::csp = NULL;
/** 线程退出标志 */
bool MyUdpClient::sendExit = false;
bool MyUdpClient::revExit = false;
static SOCKET NetSocket = 0;
static sockaddr_in RemtAddr;
static sockaddr_in LoclAddr;
static int BufLen;
static int l_naddLen1;

//构造函数中主要进行udpsocket的初始化操作
MyUdpClient::MyUdpClient() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	localPort = LOCAL_PORT;         
	remotePort = REMOTE_PORT;				

	BufLen = SENDBUFFSIZE;

	NetSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//设置远端端口
	RemtAddr.sin_family = AF_INET;
	RemtAddr.sin_port = htons(remotePort);
	inet_pton(AF_INET, REMOTE_IP, &RemtAddr.sin_addr);

	//设置本地端口
	LoclAddr.sin_family = AF_INET;
	LoclAddr.sin_port = htons(localPort);
	LoclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(NetSocket, (SOCKADDR*)&LoclAddr, sizeof(LoclAddr));

	l_naddLen1 = sizeof(LoclAddr);

	sendThread = INVALID_HANDLE_VALUE;
	revThread = INVALID_HANDLE_VALUE;
}

MyUdpClient::~MyUdpClient() {
	closesocket(NetSocket);
	WSACleanup();
	CloseRevThread();
	CloseSendThread();
}

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
	MyUdpClient * mUdpClient = reinterpret_cast<MyUdpClient*>(pParam);

	while (!mUdpClient->revExit) {
		char RevBuf[REVBUFFSIZE];
		int l_nReadLen = recvfrom(mUdpClient->NetSocket, RevBuf, mUdpClient->BufLen, 0, (struct sockaddr*)&(mUdpClient->LoclAddr), &(mUdpClient->l_naddLen1));
		if (l_nReadLen) {
			printf("recved\n");

			unsigned char data[100];
			int length = strToHex((char*)N_DIR, data);
			for (int i = 0; i < 1; ++i) {
				mUdpClient->csp->WriteData(data, length);
			}
		}
		printf("\nread:");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", RevBuf[i]);
		}

		Sleep(SLEEP_TIME_UDP);
	}

	return 0;
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
	MyUdpClient* mUdpClient = reinterpret_cast<MyUdpClient*>(pParam);

	while (!mUdpClient->sendExit) {
		char SendBuf[SENDBUFFSIZE];
		strcpy_s(SendBuf, "hello");

		int l_nLen = sendto(mUdpClient->NetSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&(mUdpClient->RemtAddr), sizeof(mUdpClient->RemtAddr));
		if (l_nLen < 0)
		{
			perror("发送失败");
		}

		printf("\nSend:");
		for (int i = 0; i < strlen(SendBuf); i++)
		{
			printf("%02x ", (UCHAR)SendBuf[i]);
		}

		Sleep(SLEEP_TIME_UDP);
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

void MyUdpClient::StartThreadTxRx() {
	bool sendStatus = OpenSendThread();
	if (!sendStatus) {
		std::cout << "send thread error\n";
	}
	else {
		std::cout << "udp send thread up\n";
	}

	bool revStatus = OpenRevThread();
	if (!revStatus) {
		std::cout << "rev thread error\n";
	}
	else {
		std::cout << "udp rev thread up\n";
	}
}

int MyUdpClient::SendPack() {
	char SendBuf[SENDBUFFSIZE];
	strcpy_s(SendBuf, "hellov1");

	int l_nLen = sendto(NetSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&RemtAddr, sizeof(RemtAddr));
	if (l_nLen < 0)
	{
		perror("发送失败");
	}

	printf("\nSend:");
	for (int i = 0; i < strlen(SendBuf); i++)
	{
		printf("%02x ", (UCHAR)SendBuf[i]);
	}
	
	return 0;
}
