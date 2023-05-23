/*main program*/
/*
1.change config.h Test or not and direction
2.get the serial can be used and change config.h
3.exe the Main
*/
#include <iostream>

//#include "MyUdpClient.h"
//#include "SerialPort.h"
#include "PortPack.h"

#include "GetPort.h"
#include "Test.h"
#include "Command.h"
#include "util.h"
#include "Config.h"

int main()
{
	std::vector<int> listCom;
	listComPortByQuery(listCom);	//GetPort have another way, but exits problem

	//CSerialPort mySerialPort;
	PortPack m_PortPack;
	if (!m_PortPack.OpenThreeRvThreads()) {
		std::cout << "Error when opening threads\n";
	}
	else {
		std::cout << "Ongoing\n";
	}


	//if (!mySerialPort.InitPort(COM_PORT))
	//{
	//	std::cout << "initPort fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	//}
	//else
	//{
	//	std::cout << "initPort success !\n\n" << std::endl;
	//}

	//if (!mySerialPort.OpenListenThread())
	//{
	//	std::cout << "OpenListenThread fail !\n\n" << std::endl;
	//}
	//else
	//{
	//	std::cout << "OpenListenThread success !\n\n" << std::endl;
	//}

	//if (!mySerialPort.OpenTerminalThread()) {
	//	std::cout << "Terminal thread fail \n\n";
	//}
	//else {
	//	std::cout << "Open terminal thread success!\n\n";
	//}

	/*unsigned char data[100];
	int length = strToHex((char*)N_DIR, data);
	for (int i = 0; i < 1; ++i) {
		mySerialPort.WriteData(data, length);
	}*/

	//MyUdpClient m_UdpClient;
	//if (!m_UdpClient.OpenRevThread()) {
	//	std::cout << "Udp rev thread error\n";
	//}
	//else {
	//	std::cout << "Udp rev thread up\n";
	//}

	//send test
	//for (int i = 0; i < 10; ++i) {
	//	m.SendPack();

	//	Sleep(1000);
	//}

	//while (1) {
	//	char RevBuf[REVBUFFSIZE];
	//	int l_nReadLen = recvfrom(m.SendSocket, RevBuf, m.BufLen, 0, (struct sockaddr*)&(m.SenderAddr), &(m.l_naddLen1));
	//	if (l_nReadLen) {
	//		printf("recved\n");

	//		//test use
	//		m.SendPack();

	//		unsigned char data[100];
	//		int length = strToHex((char*)N_DIR, data);
	//		for (int i = 0; i < 1; ++i) {
	//			mySerialPort.WriteData(data, length);
	//		}
	//	}
	//	printf("\nread£º");
	//	for (int i = 0; i < l_nReadLen; i++)
	//	{
	//		printf("%02x ", RevBuf[i]);
	//	}

	//	Sleep(1);
	//}

	while (1) {}
	return 0;
}