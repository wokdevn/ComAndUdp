﻿#include <iostream>
#include "SerialPort.h"
#include "GetPort.h"
#include "Test.h"
#include "Command.h"
#include "util.h"
#include "Config.h"

<<<<<<< HEAD
int amain()
{

=======
int main()
{
>>>>>>> 041e2bb049fbc4ff41a7495e84825a5aaefa0aef
	getComPortByQuery();

	CSerialPort mySerialPort;

	if (!mySerialPort.InitPort(7))
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

<<<<<<< HEAD
=======
	//MyUdpClient m;
	////m.setcsp(&mySerialPort);
	//m.StartThread();

>>>>>>> 041e2bb049fbc4ff41a7495e84825a5aaefa0aef
	//防止主程序退出
	while (1) {}

	return 0;
}