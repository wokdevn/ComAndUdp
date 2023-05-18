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
// Method:    strToHex ʮ�������ַ���ת16����
// FullName:  CommUtil::strToHex
// Access:    public 
// Returns:   data length
// Qualifier:
// Parameter: char * hexStr ʮ�������ַ���
// Parameter: unsigned char * outData �洢16��������
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
    /** �õ������ָ�� */
    //CSerialPort* pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

    // �߳�ѭ��,��ѯ��ʽ��ȡ��������  
    while (s_bExit)
    {
        //UINT BytesInQue = pSerialPort->GetBytesInCOM();
        ///** ����������뻺������������,����Ϣһ���ٲ�ѯ */
        //if (BytesInQue == 0)
        //{
        //    Sleep(SLEEP_TIME_INTERVAL);
        //    continue;
        //}

        ///** ��ȡ���뻺�����е����ݲ������ʾ */
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
    /** ����߳��Ƿ��Ѿ������� */
    if (m_TestThread != INVALID_HANDLE_VALUE)
    {
        /** �߳��Ѿ����� */
        std::cout << "already open\n";
        CloseHandle(m_TestThread);
        return false;
    }

    s_bExit = true;
    /** �߳�ID */
    UINT threadId;
    /** �����������ݼ����߳� */
    m_TestThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &threadId);
    if (!m_TestThread)
    {
        return false;
    }
    /** �����̵߳����ȼ�,������ͨ�߳� */
    if (!SetThreadPriority(m_TestThread, THREAD_PRIORITY_ABOVE_NORMAL))
    {
        return false;
    }

    return true;
}

