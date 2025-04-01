//
// Created by Am on 3/30/2025.
//

#ifndef C_SERIAL_USER_H
#define C_SERIAL_USER_H

#include "SerialPort.h"
#include <vector>
#include <string>

/**
 * 预指令对函数进行封装
 * 指令1：发送请求数据
 * 指令2：获取响应数据
 * 指令3：存储请求数据
 * 指令4：读取请求数据存入单片机
 *
 * 这里只支持发送2字节的命令
 * 报文头格式:包头标识 功能码 总长度 数据 结束
 * 0x24 0x01 0x10 0x01 0x03 0x00 0x00 0x00 0x02 0xC4 0X0B 0xFE
 * 其中0x24是包头，说明数据开始发送 0x01是功能码，这里用作传入请求 0x10是总长度，我觉得这个程序自己计算一下就好
 * 也就是只需要用户输入数据部分内容，最后结束占1个字节码，0xFE表示结束
 */
class User {
public:
    User();

    static SerialPort Open(const char *portname,
                    int baudrate,
                    char parity,
                    char databit,
                    char stopbit,
                    char synchronizeflag);

    static void SendDemo(SerialPort &S,const std::string &database,int id); // 基础发数据

    static void SendDemo2(SerialPort &S, const std::string &database, int id);

    void StartReceiving(SerialPort &S);

    static void ReceiveDemo(SerialPort &s); // 基础收数据

    static SerialPort CreateSP();  // 创建并打开通道

    static void SaveData(std::string &filename);

    static void ReadVec(const std::string &input, std::vector<int> &data);

    static void FileDirectory();

};


#endif //C_SERIAL_USER_H
