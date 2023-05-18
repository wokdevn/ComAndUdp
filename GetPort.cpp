#include "GetPort.h"

char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;

    //如果想要转换成string，直接赋值即可
    //string pKey = pCStrKey;
}


//TODO:fix this function
std::vector<std::string> getComPortByReg()
{
    HKEY hKey;
    wchar_t portName[256], w_commName[256];
    std::vector<std::string> comName;
    //打开串口注册表对应的键值  
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey))
    {
        int i = 0;
        int mm = 0;
        DWORD  dwLong, dwSize;
        while (TRUE)
        {
            dwLong = dwSize = sizeof(portName);
            //枚举串口
            if (ERROR_NO_MORE_ITEMS == ::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)w_commName, &dwSize))
            {
                break;
            }
            char* commName = wideCharToMultiByte(w_commName);
            comName.push_back(commName);
            delete[] commName;
            i++;
        }
        //关闭注册表
        RegCloseKey(hKey);
    }
    else
    {
        std::cout<<"您的计算机的注册表上没有HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm项\n";
    }
    //返回串口号
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