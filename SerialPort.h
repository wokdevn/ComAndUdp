//TODO:测试下,多个实例能否利用同一个COM端口,如果可以的话,那从udp那里调用可能会简单些
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <process.h>
#include <iostream>
#include "Command.h"
#include "Util.h" 

//预处理指令,这里的作用是指定链接库
#pragma comment(lib,"ws2_32.lib")

//参数设置
#define REMOTE_PORT 8001
#define LOCAL_PORT  8000
#define REMOTE_IP "192.168.0.27"
#define SENDBUFFSIZE 1024
#define REVBUFFSIZE 1024
#define SLEEP_TIME_SERIAL 1
#define SLEEP_TIME_UDP 1

using namespace std;

class CSerialPort;
class MyUdpClient
{
public:
	MyUdpClient();
	~MyUdpClient();
	static CSerialPort * csp;

public:
	bool OpenRevThread();
	bool OpenSendThread();
	bool CloseRevThread();

	bool CloseSendThread();
	void StartThreadTxRx();
	int SendPack();

	//_stdcall的方法,用于_beginthreadex调用,不能放在类中,否则得定义成static的
	static UINT WINAPI RevThreadFunc(void* pParam);
	static UINT WINAPI SendThreadFunc(void* pParam);

	static SOCKET NetSocket;
	static sockaddr_in RemtAddr;
	static sockaddr_in LoclAddr;
	static int BufLen;
	static int l_naddLen1;
	WSADATA wsaData;					 //用于启动套接字,主要是调用起来动态链接库
	int localPort = LOCAL_PORT;
	int remotePort = REMOTE_PORT;

	/** 线程退出标志变量 */
	static bool sendExit;
	static bool revExit;

	/** 线程句柄 */
	volatile HANDLE sendThread;
	volatile HANDLE revThread;
};




//TODO:测试下,多个实例能否利用同一个COM端口,如果可以的话,那从udp那里调用可能会简单些
class CSerialPort
{
public:
	CSerialPort(void);
	~CSerialPort(void);
	static MyUdpClient* muc;

public:

	/** 串口监听线程
	*
	*  监听来自串口的数据和信息
	*  @param:  void * pParam 线程参数
	*  @return: UINT WINAPI 线程返回值
	*  @note:
	*  @see:
	*/
	static UINT WINAPI ListenThreadFunc(void* pParam);

	/** 终端监听线程
	*
	*  监听来自终端的数据和信息
	*  @param:  void * pParam 线程参数
	*  @return: UINT WINAPI 线程返回值
	*  @note:
	*  @see:
	*/
	static UINT WINAPI TerminalThreadFunc(void* pParam);

	/** 初始化串口函数
	*
	*  @param:  UINT portNo 串口编号,默认值为1,即COM1,注意,尽量不要大于9
	*  @param:  UINT baud   波特率,默认为115200
	*  @param:  char parity 是否进行奇偶校验,'Y'表示需要奇偶校验,'N'表示不需要奇偶校验
	*  @param:  UINT databits 数据位的个数,默认值为8个数据位
	*  @param:  UINT stopsbits 停止位使用格式,默认值为1
	*  @param:  DWORD dwCommEvents 默认为EV_RXCHAR,即只要收发任意一个字符,则产生一个事件
	*  @return: bool  初始化是否成功
	*  @note:   在使用其他本类提供的函数前,请先调用本函数进行串口的初始化
	*　　　　　   /n本函数提供了一些常用的串口参数设置,若需要自行设置详细的DCB参数,可使用重载函数
	*           /n本串口类析构时会自动关闭串口,无需额外执行关闭串口
	*  @see:
	*/
	bool InitPort(UINT  portNo = 1, UINT  baud = CBR_115200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);

	/** 串口初始化函数
	*
	*  本函数提供直接根据DCB参数设置串口参数
	*  @param:  UINT portNo
	*  @param:  const LPDCB & plDCB
	*  @return: bool  初始化是否成功
	*  @note:   本函数提供用户自定义地串口初始化参数
	*  @see:
	*/
	bool InitPort(UINT  portNo, const LPDCB& plDCB);

	/** 开启监听线程
	*
	*  本监听线程完成对串口数据的监听,并将接收到的数据打印到屏幕输出
	*  @return: bool  操作是否成功
	*  @note:   当线程已经处于开启状态时,返回flase
	*  @see:
	*/
	bool OpenListenThread();

	/** 关闭监听线程
	*
	*
	*  @return: bool  操作是否成功
	*  @note:   调用本函数后,监听串口的线程将会被关闭
	*  @see:
	*/
	bool CloseListenTread();

	/** 向串口写数据
	*
	*  将缓冲区中的数据写入到串口
	*  @param:  unsigned char * pData 指向需要写入串口的数据缓冲区
	*  @param:  unsigned int length 需要写入的数据长度
	*  @return: bool  操作是否成功
	*  @note:   length不要大于pData所指向缓冲区的大小
	*  @see:
	*/
	bool WriteData(unsigned char* pData, unsigned int length);

	/** 开启终端输入监听线程
	*
	*  本监听线程完成对终端输入数据的监听,并将接收到的数据与码表对应并发送
	*  @return: bool  操作是否成功
	*  @note:   当线程已经处于开启状态时,返回flase
	*  @see:
	*/
	bool OpenTerminalThread();

	/** 关闭终端监听线程
	*
	*
	*  @return: bool  操作是否成功
	*  @note:   调用本函数后,监听终端的线程将会被关闭
	*  @see:
	*/
	bool CloseTerminalThread();

	/** 获取串口缓冲区中的字节数
	*
	*
	*  @return: UINT  操作是否成功
	*  @note:   当串口缓冲区中无数据时,返回0
	*  @see:
	*/
	UINT GetBytesInCOM();

	/** 读取串口接收缓冲区中一个字节的数据
	*
	*
	*  @param:  char & cRecved 存放读取数据的字符变量
	*  @return: bool  读取是否成功
	*  @note:
	*  @see:
	*/
	bool ReadChar(char& cRecved);

private:

	/** 打开串口
	*
	*
	*  @param:  UINT portNo 串口设备号
	*  @return: bool  打开是否成功
	*  @note:
	*  @see:
	*/
	bool openPort(UINT  portNo);

	/** 关闭串口
	*
	*
	*  @return: void  操作是否成功
	*  @note:
	*  @see:
	*/
	void ClosePort();

public:
	/** 线程退出标志变量 */
	static bool s_bExit;
	static bool terminalExit;

private:

	/** 串口句柄 */
	HANDLE  m_hComm;

	/** 线程句柄 */
	volatile HANDLE    m_hListenThread;
	volatile HANDLE    m_TerminalThread;

	/** 同步互斥,临界区保护 */
	CRITICAL_SECTION   m_csCommunicationSync;        //!< 互斥操作串口

};
