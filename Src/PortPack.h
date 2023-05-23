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


	/*线程退出标志*/
	static bool udpRevExit;
	static bool serialTxRevExit;
	static bool serialRxRevExit;

	/*线程句柄*/
	volatile HANDLE udpRThread;
	volatile HANDLE serialTxRThread;
	volatile HANDLE serialRxRThread;

	int rvflag;
	/** 同步互斥,临界区保护 */
	CRITICAL_SECTION   portPackCS;        //!< 互斥操作rvflag
	int sendUdp(int sig);
};
