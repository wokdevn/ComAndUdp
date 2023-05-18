#include "util.h"
#include<climits>

//************************************
// Method:    strToHex 十六进制字符串转16进制
// FullName:  CommUtil::strToHex
// Access:    public 
// Returns:   data length
// Qualifier:
// Parameter: char * hexStr 十六进制字符串
// Parameter: unsigned char * outData 存储16进制容器
//************************************
int strToHex(char* hexStr, unsigned char* outData) {
    int len = strlen(hexStr);
    //int size = sizeof(*hexStr);

    char* tmpStr = new char[strlen(hexStr) + 1];
    strcpy_s(tmpStr, strlen(hexStr) + 1, hexStr);

    char* state;
    char** statePtr = &state;

    char* res = strtok_s(tmpStr, " ", statePtr);

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
    while (res != NULL) {
        noBlank += res;
        std::cout << res;
        res = strtok_s(NULL, " ", statePtr);
    }

    std::cout << "\n";

    if (noBlank.size() % 2) {
        std::cerr << "Error: command not even num.\n";
        return -1;
    }
    
    std::cout << std::hex;
    int j = 0;
    for (int i = 0; i < noBlank.size() / 2; i++) {
        std::string twoCh = noBlank.substr(2 * i, 2);

        int r = strtoull(twoCh.c_str(), NULL, 16);
        outData[i] = r;
        std::cout << twoCh << "  " << r << " outData:" << (int)(outData[i]) << std::endl;
        j++;
    }

    return j;
}
