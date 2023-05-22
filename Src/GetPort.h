/*获取系统串口列表*/

#include <string.h>
#include <stdio.h>
#include <vector>  
#include <string>  
#include <atlbase.h>
#include <cstring>
#include <atlstr.h>
#include <iostream>
#include <vector>

//双字节类型转换
char* wideCharToMultiByte(wchar_t* pWCStrKey);

//通过遍历查询获取串口列表COM0-255查询存在否
void listComPortByQuery(std::vector<int>& list);

//通过注册表获取串口列表
std::vector<std::string> getComPortByReg();