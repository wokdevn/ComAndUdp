#include "string.h"
#include "stdio.h"
#include <vector>  
#include <string>  
#include <atlbase.h>
#include <cstring>
#include <atlstr.h>
#include <iostream>

char* wideCharToMultiByte(wchar_t* pWCStrKey);

std::vector<std::string> getComPortByReg();

void getComPortByQuery();