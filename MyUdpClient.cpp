#include "MyUdpClient.h"

//?????????????????????????构造函数无法解析的问题,为什么serial那个可以声明的时候不加大括号,vs的bug淦
//构造函数,初始化用
MyUdpClient::MyUdpClient() {
	printf("Cons\n");

	init = WSAStartup(MAKEWORD(2, 2), &wsaData);

	localPort = LOCAL_PORT;         //本地监听端口
	Port = REMOTE_PORT;				//服务器监听端口

	BufLen = SENDBUFFSIZE;

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

MyUdpClient::~MyUdpClient() {
	std::cout << "delete\n";
	closesocket(SendSocket);
	WSACleanup();
	CloseRevThread();
	CloseSendThread();
}

/** 线程退出标志 */
bool MyUdpClient::sendExit = false;
bool MyUdpClient::revExit = false;

//static变量初始化
SOCKET MyUdpClient::SendSocket = 0;
sockaddr_in MyUdpClient::RecvAddr;
sockaddr_in MyUdpClient::SenderAddr;
int MyUdpClient::BufLen;
int MyUdpClient::l_naddLen1;

const UINT SLEEP_TIME = 1;


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
		int l_nReadLen = recvfrom(SendSocket, RevBuf, BufLen, 0, (struct sockaddr*)&SenderAddr, &l_naddLen1);
		if (l_nReadLen) {
			printf("recved\n");

			unsigned char data[100];
			int length = strToHex((char*)N_DIR, data);
			for (int i = 0; i < 1; ++i) {
				//csp->WriteData(data, length);
			}
		}
		printf("\nread：");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", RevBuf[i]);
		}

		Sleep(SLEEP_TIME);
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
	MyUdpClient* mUdpClient = reinterpret_cast<MyUdpClient*>(pParam);

	while (!mUdpClient->sendExit) {
		char SendBuf[SENDBUFFSIZE];
		strcpy_s(SendBuf, "hello");

		int l_nLen = sendto(SendSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
		if (l_nLen < 0)
		{
			perror("发送失败");
		}

		printf("\nSend：");
		for (int i = 0; i < strlen(SendBuf); i++)
		{
			printf("%02x ", (UCHAR)SendBuf[i]);
		}

		Sleep(SLEEP_TIME);
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

void MyUdpClient::StartThread() {
	//bool sendStatus = OpenSendThread();
	//if (!sendStatus) {
	//	std::cout << "send thread error\n";
	//}
	//else {
	//	std::cout << "udp send thread up\n";
	//}

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
	strcpy_s(SendBuf, "hello");

	int l_nLen = sendto(SendSocket, SendBuf, strlen(SendBuf), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (l_nLen < 0)
	{
		perror("发送失败");
	}

	printf("\nSend：");
	for (int i = 0; i < strlen(SendBuf); i++)
	{
		printf("%02x ", (UCHAR)SendBuf[i]);
	}
	
	return 0;
}
