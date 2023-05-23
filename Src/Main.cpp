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
	//In case main exit;
	while (1) {}
	return 0;
}