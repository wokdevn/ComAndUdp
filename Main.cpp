/*main program*/
#include <iostream>
#include "SerialPort.h"
#include "GetPort.h"
#include "Test.h"
#include "Command.h"
#include "util.h"
#include "Config.h"

int main()
{
	std::vector<int> listCom;
	listComPortByQuery(listCom);	//GetPort have another way, but exits problem

	CSerialPort mySerialPort;

	//Serial port init
	if (!mySerialPort.InitPort(COM_PORT))
	{
		std::cout << "initPort fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	}
	else
	{
		std::cout << "initPort success !\n\n" << std::endl;
	}

	//Serial port listen
	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "OpenListenThread fail !\n\n" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !\n\n" << std::endl;
	}

	//Terminal read
	if (!mySerialPort.OpenTerminalThread()) {
		std::cout << "Terminal thread fail \n\n";
	}
	else {
		std::cout << "Open terminal thread success!\n\n";
	}

	/*unsigned char data[100];
	int length = strToHex((char*)N_DIR, data);
	for (int i = 0; i < 1; ++i) {
		mySerialPort.WriteData(data, length);
	}*/

	MyUdpClient m;
	m.csp = &mySerialPort;
	if (!m.OpenRevThread()) {
		std::cout << "Udp rev thread error\n";
	}
	else {
		std::cout << "Udp rev thread up\n";
	}

	//send test
	//for (int i = 0; i < 10; ++i) {
	//	m.SendPack();

	//	Sleep(1000);
	//}

	////Udp listen in main thread
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
	//	printf("\nread：");
	//	for (int i = 0; i < l_nReadLen; i++)
	//	{
	//		printf("%02x ", RevBuf[i]);
	//	}

	//	Sleep(1);
	//}

	//In case exit
	while (1) {}

	return 0;
}