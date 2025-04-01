//
// Created by y on 3/30/2025.
//

#ifndef C_SERIAL_PROTOCOLSAVE_H
#define C_SERIAL_PROTOCOLSAVE_H

#include <vector>
#include <string>

using namespace std;

extern int currentMaxId; //全局变量，自动分配ID

struct Device {
    int id;                           // 主键ID
    string name;                 // 设备名
    string params;               // 参数信息 例如波特率 协议什么的
    string function;             // 功能 比如 修改地址 查询数据
    vector<int> func_code;       // 功能码 发送请求的功能码
    vector<int> response;         // 响应 能得到设备的响应
};

class ProtocolSave {

public:
    ProtocolSave();

    static void SaveData(const vector<Device> &devices, const string &filename);

    static string JoinVector(const std::vector<int> &vec, const std::string &delimiter);

    static void AddDevice(vector<Device> &devices, const Device &device, const string &filename);

    static vector<Device> LoadData(const string &filename);

    static vector<int> SplitStringToVector(const std::string &str, const std::string &delimiter);

    static bool CheckAndCreateFile(const string &filename);

    static Device *FindDevice(vector<Device> &devices, int id);

    static void PrintDevices(const vector<Device> &devices);

    static void RemoveDevice(vector<Device> &devices, int id, const string &filename);
};


#endif //C_SERIAL_PROTOCOLSAVE_H
