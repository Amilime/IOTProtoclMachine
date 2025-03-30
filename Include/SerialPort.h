/**
 * 作者：Am
 * 日期：2025.3.25
 * 类名：SerialPort
 * 用途：串口读写
 */

#include <windows.h>
#ifndef C_SERIAL_SERIALPORT_H
#define C_SERIAL_SERIALPORT_H


class SerialPort {
public:
    SerialPort();
    ~SerialPort(); //折构函数
    /**
     * 参数设置
     * portname:串口名，windows是COM+数字
     * baudrate: 波特率
     * parity：检验位
     * databit：数据位 4-8（windows）
     * stopbit：停止位（一般为1位）
     * synchronizeflag（同步1、异步0，目前仅使用异步通讯0）
     */
    bool openSerial(const char* portname,
                    int baudrate,
                    char parity,
                    char databit,
                    char stopbit,
                    char synchronizeflag=0);

    void closeSerial();

    unsigned long sendData(const void *buf,int len);

    unsigned long receiveData(void *buf,int len);

private:
     HANDLE hSerial;
    char synchronizeflag;
};


#endif //C_SERIAL_SERIALPORT_H
