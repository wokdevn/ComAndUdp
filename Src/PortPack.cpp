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

	InitializeCriticalSection(&portPackCSUdp);
	InitializeCriticalSection(&portPackCSPrint);
}

PortPack::~PortPack() {
	CloseThreeRvThreads();
	DeleteCriticalSection(&portPackCSUdp);
	DeleteCriticalSection(&portPackCSPrint);
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

		printf("\nUdp recv:");
		for (int i = 0; i < l_nReadLen; i++)
		{
			printf("%02x ", RevBuf[i]);
		}
		printf("\n");

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
		/** ����������뻺������������,����Ϣһ���ٲ�ѯ */
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_SERIAL);
			continue;
		}

		/*MyUdpClient m;
		m.SendPack();*/
		/*MyUdpClient* mm = new MyUdpClient();
		mm->SendPack();*/
		

		portPack->sendUdp(SERIAL_TX_SIG);

		EnterCriticalSection(&(portPack->portPackCSPrint));
		printf("Tx serial recved>>>>>>>>>>>>> \n");
		

		/** ��ȡ���뻺�����е����ݲ������ʾ */
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
		LeaveCriticalSection(&(portPack->portPackCSPrint));
	}

	return 0;
}

UINT WINAPI PortPack::serialRevRxThreadFunc(void* pParam) {
	PortPack* portPack = reinterpret_cast<PortPack*>(pParam);

	while (!portPack->serialRxRevExit) {
		UINT BytesInQue = portPack->p_SerialportRx.GetBytesInCOM();
		/** ����������뻺������������,����Ϣһ���ٲ�ѯ */
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_SERIAL);
			continue;
		}

		/*MyUdpClient m;
		m.SendPack();*/
		/*MyUdpClient* mm = new MyUdpClient();
		mm->SendPack();*/
		
		portPack->sendUdp(SERIAL_RX_SIG);

		EnterCriticalSection(&(portPack->portPackCSPrint));
		printf("rx serial recved>>>>>>>>>>>>> \n");
		/** ��ȡ���뻺�����е����ݲ������ʾ */
		char cRecved = 0x00;
		do
		{
			cRecved = 0x00;
			if (portPack->p_SerialportRx.ReadChar(cRecved) == true)
			{
				printf("%02x", ((unsigned int)cRecved & 0xff));
				continue;
			}
		} while (--BytesInQue);
		printf("||||end RRRx>>>>>>>>\n");
		LeaveCriticalSection(&(portPack->portPackCSPrint));
	
	}

	return 0;
}

bool PortPack::CloseThreeRvThreads() {
	if (serialTxRThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */
		serialTxRevExit = true;
	}

	if (serialRxRThread != INVALID_HANDLE_VALUE)
	{
		/** ֪ͨ�߳��˳� */
		serialRxRevExit = true;
	}

	if (udpRThread != INVALID_HANDLE_VALUE) {
		udpRevExit = true;
	}

	/** �ȴ��߳��˳� */
	Sleep(10);

	/** ���߳̾����Ч */
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
		printf("Two serials listening\n");
	}
	else {
		printf("serial thread open failed\n");
		flag = false;
	}

	return flag;
}

int PortPack::sendUdp(int sig) {
	EnterCriticalSection(&portPackCSUdp);

	rvflag += sig;
	if (!rvflag) {
		p_UdpClient.SendPack();
	}
	LeaveCriticalSection(&portPackCSUdp);

	return 0;
}