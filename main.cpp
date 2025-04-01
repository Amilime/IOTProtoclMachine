#include <iostream>
#include <vector>
#include <iomanip>

#include "./Include/SerialPort.h"
#include "./Include/User.h"
#include "./Include/ProtocolSave.h"
using namespace std;


int main() {
   // User user;
  //  SerialPort port = user.CreateSP();
  //  user.SendDemo(port);

   // user.ReceiveDemo(port);

   ProtocolSave ps;
   /***
    *  struct Device1 {
        int id = 1;                           // 主键ID
        string name="温度传感器";                 // 设备名
        string params = "modbus-RTU协议，波特率默认4800，无校验位，数据位8，停止位1" ;               // 参数信息 例如波特率 协议什么的
        string function = "数据查询";             // 功能 比如 修改地址 查询数据
        vector<int> func_code = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0X0B};       // 功能码 发送请求的功能码
        vector<int> response = {0};         // 响应 能得到设备的响应
    };
    */


   // const string locate = "../SaveFile";
 //  const string filename1 ="../SaveFile/devices.amdb";

 //  ps.CheckAndCreateFile(filename1);

 //  vector<Device> devices_table = ps.LoadData(filename1); // 加载
 //  cout<< "加载devices_table："<<endl;
//   ps.PrintDevices(devices_table);

//    Device device1 ={2,"温度传感器2","modbus-RTU协议，波特率默认4800，无校验位，数据位8，停止位1",
  //                   "数据查询",{0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0X0B},
  //                   {0x01,0x03,0x04,0x01,0xE6,0xFF,0x9F,0x1B,0xA0}};

    //增加设备
   // ps.AddDevice(devices_table,device1,filename1); //这里devices_table更像一个中间件 先通过加载把文件拿给它，然后再写入需要的东西。

    //查找设备
 //   int searchId = 2;
  //  Device* foundDevice = ps.FindDevice(devices_table,searchId); //这里的devices_table是通过load加载获取了文件内容后的
  //  if(foundDevice){
  //      cout <<"\n 已找到:"<<endl;
 //       cout <<"设备名: " <<foundDevice->name <<endl;
 //   }else{
  //      cout<<"\n 没找到"<<endl;
 //   }

   // ps.SaveData(Device1,"温度传感器.txt")

    const std::string filename = "../SaveFile/devices.amdb";
    ps.CheckAndCreateFile(filename);
    std::vector<Device> devices = ps.LoadData(filename);

    Device device1 = {
            0,  // ID 不再在这里设置，将会自动分配
            "温度传感器",
            "modbus-RTU协议，波特率默认4800，无校验位，数据位8，停止位1",
            "数据查询",
            {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0X0B},
            {0x01, 0x03, 0x04, 0x01, 0xE6, 0xFF, 0x9F, 0x1B, 0xA0}
    };

    ps.AddDevice(devices, device1, filename);
    std::cout << "\nAfter Adding New Device:" << std::endl;


    return 0;
}
