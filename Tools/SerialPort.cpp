#include "../Include/SerialPort.h"

#include <iostream>

#include <winsock.h>
#include <windows.h>

using namespace std;

SerialPort::SerialPort() : hSerial(INVALID_HANDLE_VALUE) {
}

SerialPort::~SerialPort() {
    // closeSerial();
}

bool SerialPort::openSerial(const char *portname,
                            int baudrate,
                            char parity,
                            char databit,
                            char stopbit,
                            char synchronizeflag) {
    this->synchronizeflag = synchronizeflag;

    // openSerial
    hSerial = CreateFile(portname,
                         GENERIC_READ | GENERIC_WRITE, // 支持读写
                         0, // 独占方式
                         NULL, // 安全属性指针
                         OPEN_EXISTING, // 打开现有串口文件
                         FILE_FLAG_OVERLAPPED, // 0是同步方式，这里采用异步
                         NULL); // 复制文件句柄，串口必须设置NULL
    if (hSerial == (HANDLE) - 1) {
        cerr << "开启串口通信错误。" << endl;
        return false;
    }

    //配置缓冲区
    if (!SetupComm(hSerial, 4096, 4096)) {
        cerr << "缓冲区出现问题" << endl;
        return false;
    }

    //设置参数
    DCB dcbSerialParams;
    //= {0}; 改成了下面一行
    memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cerr << "设备状态获取异常。" << endl;
        closeSerial();
        return false;
    }

    //波特率数据位停止位校验位
    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = databit;
    //dcbSerialParams.StopBits = stopbit;
    //dcbSerialParams.Parity = parity;

    // 这里需要设置一下校验位判断和停止位判断
    switch(stopbit){
        case 1:
            dcbSerialParams.StopBits = ONESTOPBIT;
            break;
        case 2:
            dcbSerialParams.StopBits = TWOSTOPBITS;
            break;
        case 3:
            dcbSerialParams.StopBits = ONE5STOPBITS;
            break;
    }

    switch (parity) {
        case 0:
            dcbSerialParams.Parity = NOPARITY; //无校验
            break;
        case 1:
            dcbSerialParams.Parity = ODDPARITY; //奇校验
            break;
        case 2:
            dcbSerialParams.Parity = EVENPARITY; //偶校验
            break;
        case 3:
            dcbSerialParams.Parity = MARKPARITY; //标记校验
            break;
    }

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cerr << "参数设置出错。" << endl;
        closeSerial();
        return false;
    }

    // 设置超时处理，毫秒
    //总超时=时间系数*读写字符数+时间常量
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 1000; //间隔超时
    timeouts.ReadTotalTimeoutConstant = 5000; //时间常量
    timeouts.ReadTotalTimeoutMultiplier = 500; //时间系数
    timeouts.WriteTotalTimeoutConstant = 2000; //写时间常量
    timeouts.WriteTotalTimeoutMultiplier = 1000;//写时间系数

    SetCommTimeouts(hSerial, &timeouts);

    PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);// 清空缓冲区

    return true;
}

void SerialPort::closeSerial() {


    //if (hSerial != INVALID_HANDLE_VALUE) {
     CloseHandle(hSerial);
    //    hSerial = INVALID_HANDLE_VALUE;
   // }
}

unsigned long SerialPort::sendData(const void *buf, int len) {
    DWORD bytesWritten = len;// 成功写入数据字节数
    DWORD ErrorFlags; // 错误标志
    COMSTAT comstat;  // 通讯状态
    OVERLAPPED m_osWrite; //异步输入输出结构体

    //创建一个用于OVERLAPPED的事件处理
    memset(&m_osWrite, 0, sizeof(m_osWrite));
    //没懂这里是什么意思
        m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    ClearCommError(hSerial, &ErrorFlags, &comstat); // 清除通信错误，获得设备状态

    BOOL writestat = WriteFile(hSerial,
                               buf, //数据首地址
                               bytesWritten, //要发送的数据字节数
                               &bytesWritten, //DWORD *接收成功发送的数据字节数
                               &m_osWrite); //NULL是同步发送，异步需要OVERLAPPED*


    if (!writestat) {

            if (GetLastError() == ERROR_IO_PENDING){// 如果正在写入
                WaitForSingleObject(m_osWrite.hEvent, 1000);
            } //等待写入一秒钟
            else {
                ClearCommError(hSerial, &ErrorFlags, &comstat);
                CloseHandle(m_osWrite.hEvent);
                cerr << "写入通信端口错误" << endl;
                return 0;
            }
    }
    return bytesWritten;
}

unsigned long SerialPort::receiveData(void *buf, int len) {
    DWORD bytesRead = len; //成功读取的字节数
    DWORD ErrorFlags; //错误标志
    COMSTAT comstat; //通信状态
    OVERLAPPED m_osRead; //异步输入输出结构体

    //创建一个用于OVERLAPPED的事件处理
    memset(&m_osRead, 0, sizeof(m_osRead));
    //没懂这里是什么意思
    m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    ClearCommError(hSerial, &ErrorFlags, &comstat); //清除通讯错误，获得设备当前状态
    if (!comstat.cbInQue)return 0; //如果输入缓冲区字节数为0，则返回false

    BOOL ReadStat = ReadFile(hSerial,
                             buf,
                             bytesRead,
                             &bytesRead,
                             &m_osRead);

    if (!ReadStat) {
        if(GetLastError() == ERROR_IO_PENDING){ //如果串口正在读取
            GetOverlappedResult(hSerial,&m_osRead,&bytesRead,TRUE);
            //函数一直等待，直到操作完成或由于错误返回
        }
        else {
            ClearCommError(hSerial, &ErrorFlags, &comstat);
            CloseHandle(m_osRead.hEvent);
            cerr << "读取错误" << endl;
            return 0;
        }
    }
    return bytesRead;
}