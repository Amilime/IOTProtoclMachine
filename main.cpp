#include <iostream>
#include <vector>
#include <iomanip>

#include "./Include/SerialPort.h"
#include "./Include/User.h"
#include "./Include/ProtocolSave.h"
#include "./Include/CommandHandler.h"
using namespace std;


int main() {
    cout<<"请选择你需要执行的命令"<<endl;

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
 //这里开始制定语句
   /***
    *查询现有的所有数据库
    *QUERY ALL
    * 查询某个库中的所有信息
    * QUERY {DBNAME}
    * 查询某个库中某个id的信息
    * QUERY {DBNAME} WHERE ID={NUM}
    * 开启存储数据到某个库中功能
    * SAVEDATA TO {DBNAME}
    *
    *
    * 开启串口通信
    * OPEN SP
    * 开启接收通道
    * OPEN RC
    * 发送数据库的具体功能码到串口通信
    * SEND REQ FROM {DBNAME} WHERE ID={NUM}
    * 将接收到的数据存入数据库
    * -----暂未实现-----
    * 将请求功能码和响应码都发送到串口通信
    *SEND REQ&RES FROM {DBNAME} WHERE ID={NUM}
    */


     CommandHandler commandHandler;
     string inputCommand;
     while(true){
         cout <<"请输入命令: ";
         getline(cin,inputCommand);

         if(inputCommand == "EXIT") {
             break;
         }

         CommandType commandType = commandHandler.parseCommand(inputCommand);



         commandHandler.executeCommand(commandType,inputCommand);
     }
    return 0;
}
