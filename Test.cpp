#include "Test.h"
#include <process.h>
#include <Windows.h>
#include "Command.h"
#include <string>

volatile HANDLE   m_TestThread = INVALID_HANDLE_VALUE;

void SleepTest() {
	for (int i = 0; i < 1000; ++i) {
		Sleep(1000);
		std::cout << i << " :i sleep\n";
	}
}

void CommandTest() {
	//std::cout << TEST_COMMAND <<"\n";
	//std::cout << "command size: " << sizeof(TEST_COMMAND)<<"\n\n";
}

//************************************
// Method:    strToHex 十六进制字符串转16进制
// FullName:  CommUtil::strToHex
// Access:    public 
// Returns:   data length
// Qualifier:
// Parameter: char * hexStr 十六进制字符串
// Parameter: unsigned char * outData 存储16进制容器
//************************************
int StrToHexTest(char* hexStr, unsigned char* outData) {
    int len = strlen(hexStr);
    //int size = sizeof(*hexStr);

    char* tmpStr = new char[strlen(hexStr)+1];
    strcpy_s(tmpStr, strlen(hexStr)+1, hexStr);

    char* state;
    char** statePtr = &state;

    char* res = strtok_s(tmpStr, " ",statePtr);

    //int tmpXor = 0x00;
    //int i = 0;
    //while (res != NULL) {
    //    int r = strtoull(res, NULL, 16);
    //    outData[i] = r;
    //    tmpXor ^= r;
    //    //std::cout << res << "  " << r << " outData:" << ((int)outData[i]) << std::endl;
    //    i++;

    //    res = strtok_s(NULL, " ",statePtr);
    //}

    std::string noBlank = "";
    while(res != NULL) {
        noBlank += res;
        std::cout << res << "  ";
        res = strtok_s(NULL, " ",statePtr);
    }

    std::cout << "\n";

    if (noBlank.size() % 2) {
        std::cerr << "Error: command not even num.\n";
        return -1;
    }

    int j = 0;
    for (int i = 0; i < noBlank.size()/2 ; i++) {
        std::string twoCh = noBlank.substr(2*i,2);

        int r = strtoull(twoCh.c_str(), NULL, 16);
        outData[i] = r;
        std::cout << twoCh << "  " << r << " outData:" << ((int)outData[i]) << std::endl;
        j++;
    }

    return j;
}

void ConvertTest() {
    unsigned char data[100];
    int length = StrToHexTest((char *)"01 23 45 67 89 ab cd ef gh aaab bcdd", data);
    std::cout << length;
}


bool s_bExit;
UINT WINAPI ThreadFunc(void* pParam)
{
    /** 得到本类的指针 */
    //CSerialPort* pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

    // 线程循环,轮询方式读取串口数据  
    while (s_bExit)
    {
        //UINT BytesInQue = pSerialPort->GetBytesInCOM();
        ///** 如果串口输入缓冲区中无数据,则休息一会再查询 */
        //if (BytesInQue == 0)
        //{
        //    Sleep(SLEEP_TIME_INTERVAL);
        //    continue;
        //}

        ///** 读取输入缓冲区中的数据并输出显示 */
        //char cRecved = 0x00;
        //do
        //{
        //    cRecved = 0x00;
        //    if (pSerialPort->ReadChar(cRecved) == true)
        //    {
        //        std::cout << cRecved;
        //        continue;
        //    }
        //} while (--BytesInQue);
        int temp;
        std::cin >> temp;
        std::cout << temp;
        Sleep(1000);
    }

    return 0;
}

bool OpenThreadTest()
{
    /** 检测线程是否已经开启了 */
    if (m_TestThread != INVALID_HANDLE_VALUE)
    {
        /** 线程已经开启 */
        std::cout << "already open\n";
        CloseHandle(m_TestThread);
        return false;
    }

    s_bExit = true;
    /** 线程ID */
    UINT threadId;
    /** 开启串口数据监听线程 */
    m_TestThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &threadId);
    if (!m_TestThread)
    {
        return false;
    }
    /** 设置线程的优先级,高于普通线程 */
    if (!SetThreadPriority(m_TestThread, THREAD_PRIORITY_ABOVE_NORMAL))
    {
        return false;
    }

    return true;
}

