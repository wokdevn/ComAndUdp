/*��ȡϵͳ�����б�*/

#include <string.h>
#include <stdio.h>
#include <vector>  
#include <string>  
#include <atlbase.h>
#include <cstring>
#include <atlstr.h>
#include <iostream>
#include <vector>

//˫�ֽ�����ת��
char* wideCharToMultiByte(wchar_t* pWCStrKey);

//ͨ��������ѯ��ȡ�����б�COM0-255��ѯ���ڷ�
void listComPortByQuery(std::vector<int>& list);

//ͨ��ע����ȡ�����б�
std::vector<std::string> getComPortByReg();