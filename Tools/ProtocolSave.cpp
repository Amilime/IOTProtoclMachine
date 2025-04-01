//
// Created by y on 3/30/2025.
//

#include "../Include/ProtocolSave.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std;

int currentMaxId = 0;

ProtocolSave::ProtocolSave() {

}

//存储底层

void ProtocolSave::SaveData(const vector<Device> &devices, const std::string &filename) {
    ofstream ofs(filename);
    if (!ofs) {
        throw runtime_error("打开或写入文件错误");
    }
    for (const auto &device: devices) {
        ofs << device.id << "," << device.name << "," << device.params << ","
            << device.function << "," << JoinVector(device.func_code, ";") << ","
            << JoinVector(device.response, ";") << endl;
    }
}

string ProtocolSave::JoinVector(const std::vector<int> &vec, const std::string &delimiter) {
    ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i < vec.size() - 1) {
            oss << delimiter;
        }
    }
    return oss.str();
}

vector<Device> ProtocolSave::LoadData(const string &filename) {
    vector<Device> devices;
    ifstream ifs(filename);
    if (!ifs) {
        throw runtime_error("打开或读取文件出错");
    }
    string line;
    while (getline(ifs, line)) {
        istringstream ss(line);
        Device device;
        string token;

        getline(ss, token, ',');
        device.id = stoi(token);
        getline(ss, device.name, ',');
        getline(ss, device.params, ',');
        getline(ss, device.function, ',');

        getline(ss, token, ',');  // 读取功能码
        device.func_code = SplitStringToVector(token, ";");  //调试执行到这里报错

        getline(ss, token, ',');  // 读取响应
        device.response = SplitStringToVector(token, ";");

        devices.push_back(device);
        if(device.id > currentMaxId) {
            currentMaxId = device.id;
        }

    }
    return devices;
}

vector<int> ProtocolSave::SplitStringToVector(const string &str, const string &delimiter) {
    vector<int> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find(delimiter, start)) != string::npos) {
        result.push_back(stoi(str.substr(start, end - start)));
        start = end + delimiter.length();
    }
    result.push_back(stoi(str.substr(start)));
    return result;
}

bool ProtocolSave::CheckAndCreateFile(const string &filename) {
    ifstream infile(filename);
    if (!infile) {
        ofstream outfile(filename);
        outfile.close();
        return false;
    }
    return true;
}

// 下面是增删改查的实现

//这里的三个参数分别是，devices是存入最终filename的东西  device是需要添加到devices里的行数据
void ProtocolSave::AddDevice(vector<Device> &devices, const Device &device, const string &filename) {
    Device newDevice = device;
    newDevice.id = ++currentMaxId; //自动分配ID
    devices.push_back(newDevice);
    SaveData(devices, filename);
}

//查
Device *ProtocolSave::FindDevice(vector<Device> &devices, int id) {
    for (auto &device: devices) {  //这里就直接用建议的顺序查找来查了，后续可以修改
        if (device.id == id) {
            return &device; // 返回找到的设备指针
        }
    }
    return nullptr;
}

void ProtocolSave::PrintDevices(const vector<Device> &devices) {
    for (const auto &device: devices) {
        cout <<  " 序号:" << device.id
             << ", 名字: " << device.name
             << ", 参数: " << device.params
             << ", 功能: " << device.function
             << ", 功能码: ";
        for (const int& code : device.func_code) {
            cout<<code<<" ";
        }
        cout<<", 响应: ";
        for (const int& resp : device.response) {
            cout<<resp << " ";
        }
        cout<<endl;
    }
}

void ProtocolSave::RemoveDevice(vector<Device> &devices, int id, const string &filename) {
    auto it = remove_if(devices.begin(),devices.end(),[id](const Device& d){
        return d.id == id;
    });
    devices.erase(it,devices.end());
    SaveData(devices,filename); //更新文件

}



