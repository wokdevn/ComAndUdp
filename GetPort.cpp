#include "GetPort.h"

char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;

    //�����Ҫת����string��ֱ�Ӹ�ֵ����
    //string pKey = pCStrKey;
}


//TODO:fix this function
std::vector<std::string> getComPortByReg()
{
    HKEY hKey;
    wchar_t portName[256], w_commName[256];
    std::vector<std::string> comName;
    //�򿪴���ע����Ӧ�ļ�ֵ  
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey))
    {
        int i = 0;
        int mm = 0;
        DWORD  dwLong, dwSize;
        while (TRUE)
        {
            dwLong = dwSize = sizeof(portName);
            //ö�ٴ���
            if (ERROR_NO_MORE_ITEMS == ::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)w_commName, &dwSize))
            {
                break;
            }
            char* commName = wideCharToMultiByte(w_commName);
            comName.push_back(commName);
            delete[] commName;
            i++;
        }
        //�ر�ע���
        RegCloseKey(hKey);
    }
    else
    {
        std::cout<<"���ļ������ע�����û��HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm��\n";
    }
    //���ش��ں�
    return comName;
}

void getComPortByQuery() //added function to find the present serial 
{
    std::cout << "COM device on PC:\n";
    TCHAR lpTargetPath[5000]; // buffer to store the path of the COMPORTS
    DWORD test;
    bool gotPort = 0; // in case the port is not found

    for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
    {
        CString str;
        str.Format(_T("%d"), i);
        CString ComName = CString("COM") + CString(str); // converting to COM0, COM1, COM2

        test = QueryDosDevice(ComName, lpTargetPath, 5000);

        // Test the return value and error if any
        if (test != 0) //QueryDosDevice returns zero if it didn't find an object
        {
            _tprintf(_T("%s\n"), (LPCTSTR)ComName); // add to the ComboBox
            gotPort = 1; // found port
        }

        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            lpTargetPath[10000]; // in case the buffer got filled, increase size of the buffer.
            continue;
        }

    }

    if (!gotPort) // if not port
        std::cout<<"No Active Ports Found\n\n"; // to display error message incase no ports found

}