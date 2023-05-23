#include "MyUdpClient.h"
#include "SerialPort.h"

class PortPack{
public:
	MyUdpClient p_UdpClient;
	CSerialPort p_Serialport;

	PortPack();
	~PortPack();

	bool OpenTwoRvThreads();
	bool CloseTwoRvThreads();

	bool openUdpRvThread();
	bool openSerialRvThread();
	//bool closeUdpRvThread();
	//bool closeSerialRvThread();

	static UINT WINAPI udpRevThreadFunc(void* pParam);
	static UINT WINAPI serialRevThreadFunc(void* pParam);


	/*�߳��˳���־*/
	static bool udpRevExit;
	static bool serialRevExit;

	/*�߳̾��*/
	volatile HANDLE udpRThread;
	volatile HANDLE serialRThread;
};
