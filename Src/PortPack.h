#include "MyUdpClient.h"
#include "SerialPort.h"

#define SERIAL_TX_SIG 1
#define SERIAL_RX_SIG -1

#define SERIAL_TX_FUNC 1
#define SERIAL_RX_FUNC 2

class PortPack{
public:
	MyUdpClient p_UdpClient;
	CSerialPort p_SerialportTx;
	CSerialPort p_SerialportRx;

	//int serialFuncId;

	PortPack();
	~PortPack();

	bool OpenThreeRvThreads();
	bool CloseThreeRvThreads();

	bool openUdpRvThread();
	bool openTwoSerialRvThread();

	//bool closeUdpRvThread();
	//bool closeSerialRvThread();

	static UINT WINAPI udpRevThreadFunc(void* pParam);
	static UINT WINAPI serialRevTxThreadFunc(void* pParam);
	static UINT WINAPI serialRevRxThreadFunc(void* pParam);


	/*�߳��˳���־*/
	static bool udpRevExit;
	static bool serialTxRevExit;
	static bool serialRxRevExit;

	/*�߳̾��*/
	volatile HANDLE udpRThread;
	volatile HANDLE serialTxRThread;
	volatile HANDLE serialRxRThread;

	int rvflag;
	/** ͬ������,�ٽ������� */
	CRITICAL_SECTION   portPackCS;        //!< �������rvflag
	int sendUdp(int sig);
};
