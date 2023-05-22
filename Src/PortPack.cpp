#include "PortPack.h"

bool PortPack::udpRevExit = false;
bool PortPack::serialRevExit = false;

PortPack::PortPack() {
	udpRThread = INVALID_HANDLE_VALUE;
	serialRThread = INVALID_HANDLE_VALUE;

	if (!p_Serialport.InitPort(COM_PORT))
	{
		std::cout << "initPort fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	}
	else
	{
		std::cout << "initPort success !\n\n" << std::endl;
	}
}

PortPack::~PortPack() {
	CloseTwoRvThreads();
}

bool PortPack::openUdpRvThread() {
	if (udpRThread != INVALID_HANDLE_VALUE) {
		std::cout << "UDPRev thread already open\n\n";
		return false;
	}

	udpRevExit = false;

	UINT u_threadId;

	udpRThread = (HANDLE)_beginthreadex(NULL, 0, udpRevThreadFunc, this, 0, &u_threadId);

	if (!udpRThread) {
		return false;
	}

	if (!SetThreadPriority(udpRThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

UINT WINAPI PortPack::udpRevThreadFunc(void* pParam) {
	PortPack* portPack = reinterpret_cast<PortPack*>(pParam);

	while (!portPack->udpRevExit) {
		char RevBuf[REVBUFFSIZE];
		int l_nReadLen = recvfrom(portPack->p_UdpClient.NetSocket, 
			RevBuf,
			portPack->p_UdpClient.BufLen, 
			0,
			(struct sockaddr*)&(portPack->p_UdpClient.LoclAddr),
			&(portPack->p_UdpClient.l_naddLen1));
		if (l_nReadLen) {
			printf("recved\n");

			unsigned char data[100];
			int length = strToHex((char*)TEST_NEW_COMMAND, data);
			for (int i = 0; i < 1; ++i) {
				//mUdpClient->csp->WriteData(data, length);
				portPack->p_Serialport.WriteData(data, length);
			}
		}

		//test use
		//portPack->p_UdpClient.SendPack();

		printf("\nread:");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", RevBuf[i]);
		}

		Sleep(SLEEP_TIME_UDP);
	}

	return 0;
}

bool PortPack::openSerialRvThread() {
	if (serialRThread != INVALID_HANDLE_VALUE) {
		std::cout << "SerialRev thread already open\n\n";
		return false;
	}

	serialRevExit = false;

	UINT s_threadId;

	serialRThread = (HANDLE)_beginthreadex(NULL, 0, serialRevThreadFunc, this, 0, &s_threadId);

	if (!serialRThread) {
		return false;
	}

	if (!SetThreadPriority(serialRThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

UINT WINAPI PortPack::serialRevThreadFunc(void* pParam) {
	PortPack* portPack = reinterpret_cast<PortPack*>(pParam);

	while (!portPack->serialRevExit) {
		UINT BytesInQue = portPack->p_Serialport.GetBytesInCOM();
		/** 如果串口输入缓冲区中无数据,则休息一会再查询 */
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_SERIAL);
			continue;
		}

		/*MyUdpClient m;
		m.SendPack();*/
		/*MyUdpClient* mm = new MyUdpClient();
		mm->SendPack();*/
		printf("serial reved \n");
		portPack->p_UdpClient.SendPack();

		/** 读取输入缓冲区中的数据并输出显示 */
		char cRecved = 0x00;
		do
		{
			cRecved = 0x00;
			if (portPack->p_Serialport.ReadChar(cRecved) == true)
			{
				printf("%02x", ((unsigned int)cRecved & 0xff));
				continue;
			}
		} while (--BytesInQue);
		printf("||||end>>>>>>>>\n");
	}

	return 0;
}

bool PortPack::CloseTwoRvThreads() {
	if (serialRThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		serialRevExit = true;
	}
	if (udpRThread != INVALID_HANDLE_VALUE) {
		udpRevExit = true;
	}

	/** 等待线程退出 */
	Sleep(10);

	/** 置线程句柄无效 */
	CloseHandle(serialRThread);
	serialRThread = INVALID_HANDLE_VALUE;

	CloseHandle(udpRThread);
	udpRThread = INVALID_HANDLE_VALUE;

	return true;
}

bool PortPack::OpenTwoRvThreads() {
	bool flag = true;
	if (openUdpRvThread()) {
		printf("udp listening\n");
	}
	else {
		printf("udp thread open failed\n");
		flag = false;
	}

	if (openSerialRvThread()) {
		printf("serial listening\n");
	}
	else {
		printf("serial thread open failed\n");
		flag = false;
	}

	return flag;
}
