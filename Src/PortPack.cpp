#include "PortPack.h"

bool PortPack::udpRevExit = false;
bool PortPack::serialTxRevExit = false;
bool PortPack::serialRxRevExit = false;

PortPack::PortPack() {
	udpRThread = INVALID_HANDLE_VALUE;
	serialTxRThread = INVALID_HANDLE_VALUE;
	serialRxRThread = INVALID_HANDLE_VALUE;

	rvflag = 0;

	if (!p_SerialportTx.InitPort(COM_PORT_TX))
	{
		std::cout << "initPort tx fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	}
	else
	{
		std::cout << "initPort tx success !\n\n" << std::endl;
	}

	if (!p_SerialportRx.InitPort(COM_PORT_RX))
	{
		std::cout << "initPort rx fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	}
	else
	{
		std::cout << "initPort rx success !\n\n" << std::endl;
	}

	InitializeCriticalSection(&portPackCS);
}

PortPack::~PortPack() {
	CloseThreeRvThreads();
	DeleteCriticalSection(&portPackCS);
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

			unsigned char txdata[100];
			unsigned char rxdata[100];
			int txlength = strToHex((char*)TEST_TX_COMMAND, txdata);
			int rxlength = strToHex((char*)TEST_RX_COMMAND, rxdata);

			portPack->p_SerialportTx.WriteData(txdata, txlength);
			portPack->p_SerialportRx.WriteData(rxdata, rxlength);
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

bool PortPack::openTwoSerialRvThread() {
	int rflag = true;
	if (serialTxRThread != INVALID_HANDLE_VALUE) {
		std::cout << "SerialRev tx thread already open\n\n";
		rflag = false;
	}

	if (serialRxRThread != INVALID_HANDLE_VALUE) {
		std::cout << "SerialRev rx thread already open\n\n";
		rflag = false;
	}

	serialTxRevExit = false;
	serialRxRevExit = false;

	UINT s_threadIdtx;
	UINT s_threadIdrx;

	serialTxRThread = (HANDLE)_beginthreadex(NULL, 0, serialRevTxThreadFunc, this, 0, &s_threadIdtx);
	serialRxRThread = (HANDLE)_beginthreadex(NULL, 0, serialRevRxThreadFunc, this, 0, &s_threadIdrx);

	if (!serialTxRThread) {
		rflag = false;
	}

	if (!serialRxRThread) {
		rflag = false;
	}

	if (!SetThreadPriority(serialTxRThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		rflag = false;
	}

	if (!SetThreadPriority(serialRxRThread, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		rflag = false;
	}

	return rflag;
}

UINT WINAPI PortPack::serialRevTxThreadFunc(void* pParam) {
	PortPack* portPack = reinterpret_cast<PortPack*>(pParam);

	while (!portPack->serialTxRevExit) {
		UINT BytesInQue = portPack->p_SerialportTx.GetBytesInCOM();
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
		printf("tx serial reved \n");

		portPack->sendUdp(SERIAL_TX_SIG);

		/** 读取输入缓冲区中的数据并输出显示 */
		char cRecved = 0x00;
		do
		{
			cRecved = 0x00;
			if (portPack->p_SerialportTx.ReadChar(cRecved) == true)
			{
				printf("%02x", ((unsigned int)cRecved & 0xff));
				continue;
			}
		} while (--BytesInQue);
		printf("||||end tttx>>>>>>>>\n");
	}

	return 0;
}

UINT WINAPI PortPack::serialRevRxThreadFunc(void* pParam) {
	PortPack* portPack = reinterpret_cast<PortPack*>(pParam);

	while (!portPack->serialRxRevExit) {
		UINT BytesInQue = portPack->p_SerialportRx.GetBytesInCOM();
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
		printf("rx serial recved \n");

		portPack->sendUdp(SERIAL_RX_SIG);

		/** 读取输入缓冲区中的数据并输出显示 */
		char cRecved = 0x00;
		do
		{
			cRecved = 0x00;
			if (portPack->p_SerialportTx.ReadChar(cRecved) == true)
			{
				printf("%02x", ((unsigned int)cRecved & 0xff));
				continue;
			}
		} while (--BytesInQue);
		printf("||||end RRRx>>>>>>>>\n");
	}

	return 0;
}

bool PortPack::CloseThreeRvThreads() {
	if (serialTxRThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		serialTxRevExit = true;
	}

	if (serialRxRThread != INVALID_HANDLE_VALUE)
	{
		/** 通知线程退出 */
		serialRxRevExit = true;
	}

	if (udpRThread != INVALID_HANDLE_VALUE) {
		udpRevExit = true;
	}

	/** 等待线程退出 */
	Sleep(10);

	/** 置线程句柄无效 */
	CloseHandle(serialTxRThread);
	serialTxRThread = INVALID_HANDLE_VALUE;

	CloseHandle(serialRxRThread);
	serialRxRThread = INVALID_HANDLE_VALUE;

	CloseHandle(udpRThread);
	udpRThread = INVALID_HANDLE_VALUE;

	return true;
}

bool PortPack::OpenThreeRvThreads() {
	bool flag = true;
	if (openUdpRvThread()) {
		printf("udp listening\n");
	}
	else {
		printf("udp thread open failed\n");
		flag = false;
	}

	if (openTwoSerialRvThread()) {
		printf("serials listening\n");
	}
	else {
		printf("serial thread open failed\n");
		flag = false;
	}

	return flag;
}

int PortPack::sendUdp(int sig) {
	EnterCriticalSection(&portPackCS);

	rvflag += sig;
	if (!rvflag) {
		p_UdpClient.SendPack();
	}
	LeaveCriticalSection(&portPackCS);

	return 0;
}