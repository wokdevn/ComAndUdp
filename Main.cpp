/*主程序*/

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
	listComPortByQuery(listCom);	//GetPort中还有一个通过注册表的方法,但是之前测试有问题

	CSerialPort mySerialPort;

	if (!mySerialPort.InitPort(COM_PORT))
	{
		std::cout << "initPort fail !!!!!!!!!!!!!!!\n\n" << std::endl;
	}
	else
	{
		std::cout << "initPort success !\n\n" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "OpenListenThread fail !\n\n" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !\n\n" << std::endl;
	}

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

	//MyUdpClient m;
	////m.setcsp(&mySerialPort);
	//m.StartThread();

	//防止主程序退出
	while (1) {}

	return 0;
}