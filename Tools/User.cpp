//
// Created by y on 3/30/2025.
//

#include "../Include/User.h"
#include "../Include/ProtocolSave.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std;


User::User() {

}


SerialPort User::Open(const char *portname,
                      int baudrate,
                      char parity,
                      char databit,
                      char stopbit,
                      char synchronizeflag) {

    SerialPort S;
    // 实际使用参数
    int State = S.openSerial(portname, baudrate, parity, databit, stopbit, synchronizeflag);

    if (State) {
        return S; // 返回打开的串口对象
    } else {
        // 处理打开失败的情况
        throw runtime_error("Failed to open serial port");
    }

}

void User::SendDemo(SerialPort &S,const string &database,int id) {

    ifstream inFile(database);
    if(!inFile.is_open()){
        cerr <<"打开数据库错误:"<<database<<endl;
        return;
    }

    string line;
    vector<int> data;

    for (int i = 1; i <= id; ++i) {
        if(!getline(inFile,line)){
            cerr << "无法读取到id为 " << id << " 的数据" << endl;
            inFile.close();
            return;
        }
    }
        stringstream ss(line);
        string temp;

    //分割
    for (int i = 0; i < 4; ++i) {
        if (!getline(ss, temp, i<5 ? ',': '\n')) {
            cerr << "行格式有误，不够字段" << endl;
            inFile.close();
            return; // 行格式有误，提前退出
        }
    }

    //读取
    if (getline(ss, temp, ',')) { // 读取第五个字段
        stringstream dataStream(temp);
        string dataItem;

        while(getline(dataStream,dataItem,';')){
            if(!dataItem.empty()){
                data.push_back(stoi(dataItem)); //整形添加到data中
            }
        }
    }
    const int func[]{0x01}; //功能码池
    int start = 0x24;
 //   vector<int> data{0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0X0B, 0xFE};
    int length = 0x08;
    int end = 0xEF;

    S.sendData(&start, 1);
    S.sendData(&func[0], 1);
    S.sendData(&length, 1);
    cout << "send: " << endl;
    for (int &i: data) {
        cout << hex << ' ' << i;
        S.sendData(&i, 1); //这里本该用sizeof 但是获取到的int是四个字节，属于是设计缺陷，后续应该改成无符号整型

        // cout <<"send demo finished"<<endl;
    }
    S.sendData(&end,1);
    cout << endl;
    inFile.close();

}

void User::SendDemo2(SerialPort &S, const string &database, int id) {
    ifstream inFile(database);
    if (!inFile.is_open()) {
        cerr << "打开数据库错误: " << database << endl;
        return;
    }

    string line;
    vector<int> data1, data2; // 用于存储第五个和第六个字段的数据

    for (int i = 1; i <= id; ++i) {
        if (!getline(inFile, line)) {
            cerr << "无法读取到id为 " << id << " 的数据" << endl;
            inFile.close();
            return;
        }
    }

    stringstream ss(line);
    string temp;

    // 分割字段
    for (int i = 0; i < 6; ++i) {
        if (!getline(ss, temp, i < 5 ? ',' : '\n')) {
            cerr << "行格式有误，不够字段" << endl;
            inFile.close();
            return; // 提前退出
        }

        // 处理第五个字段
        if (i == 4) {
            stringstream dataStream(temp);
            string dataItem;
            while (getline(dataStream, dataItem, ';')) {
                if (!dataItem.empty()) {
                    data1.push_back(stoi(dataItem)); // 整形添加到data1中
                }
            }
        }

        // 处理第六个字段
        if (i == 5) {
            stringstream dataStream(temp);
            string dataItem;
            while (getline(dataStream, dataItem, ';')) {
                if (!dataItem.empty()) {
                    data2.push_back(stoi(dataItem)); // 整形添加到data2中
                }
            }
        }
    }

    const int func[]{0x02}; // 功能码池
    int start = 0x24;
    int length = 0x08;
    int end = 0xEF;

    S.sendData(&start, 1);
    S.sendData(&func[0], 1);
    S.sendData(&length, 1);

    cout << "send: " << endl;

    // 发送第一个字段数据
    for (int &i : data1) {
        cout << hex << ' ' << i;
        S.sendData(&i, 1); // 发送整数
    }

    // 发送间隔标识
    const int interval = 0x99;
    S.sendData(&interval, 1);

    // 发送第二个字段数据
    for (int &i : data2) {
        cout << hex << ' ' << i;
        S.sendData(&i, 1); // 发送整数
    }

    S.sendData(&end, 1);
    cout << endl;
    inFile.close();
}

void User::ReceiveDemo(SerialPort &S) {


    char buf[4096];
    while (true) {
        memset(buf, 0, sizeof(buf));
        unsigned long bytesRead = S.receiveData(buf, sizeof(buf));
        if (bytesRead > 0) {
            cout << "Buffer Content (Hex): ";
            unsigned char *p = reinterpret_cast<unsigned char *>(buf);
            for (DWORD i = 0; i < bytesRead; ++i) {
                cout << hex << setw(2) << setfill('0') << (int) p[i] << " ";
            }
            cout << dec << endl;

        }
        Sleep(1000);
    }
}

// 第二层封装
//串口开启
SerialPort User::CreateSP() {
    string portname;
    int baudrate;
    int parity;
    int databit;
    int stopbit;
    int synchronizeflag;
    bool res = false;

    User user;

    while (!res) {
        cout << "是否使用预设? [y/n]:" << endl;
        char state;
        cin >> state;
        if (state == 'y') {
            cout << "选择预设: [序号: 1,] :" << endl;
            int num;
            cin >> num;
            // 这里预设较少所以暂时使用if
            if (num == 1) {
                portname = "COM1";
                baudrate = 9600;
                parity = 0;
                databit = 8;
                stopbit = 1;
                synchronizeflag = 0;
            } else {
                cout << "没有此序号" << endl;
                continue;
            }
            //这里传入参数Open()并将上面的参数赋值上去


        } else if (state == 'n') {
            cout << "放弃使用预设" << endl;

            cout << "输入串口名[windows以COM+数字起名]: " << endl;
            cin >> portname;
            cout << "输入波特率[4800,9600,115200 ...]: " << endl;
            cin >> baudrate;
            cout << "校验位  [0:无校验, 1:奇校验, 2:偶校验, 3:标记校验]:" << endl;
            cin >> parity;

            cout << "数据位 [ 4 - 8, 一般填 8]: " << endl;
            cin >> databit;

            cout << "停止位  [ 1:一位, 2:两位, 3:一点五位]: " << endl;
            cin >> stopbit;

            cout << "同步或异步 [同步1, 异步0]:" << endl;
            cin >> synchronizeflag;


        } else {
            cout << "错误的输入值，请输入 'y' 或 'n'" << endl;
            continue;
        }

        try {
            parity = static_cast<char>(parity);
            databit = static_cast<char>(databit);
            stopbit = static_cast<char>(stopbit);
            synchronizeflag = static_cast<char>(synchronizeflag);
            SerialPort S = user.Open(portname.c_str(), baudrate, parity, databit, stopbit, synchronizeflag);
            cout << "串口开启成功:" << portname << endl;
            res = true;
            return S;

        } catch (const runtime_error &e) {
            cout << e.what() << endl;
            cout << "请重新输入参数" << endl;
        }

    }
}

// 数据存储
void User::SaveData(string &filename) {

  //  cout << "输入你要存储/创建的库[不用写后缀]:" << endl;

    ProtocolSave ps;
    ps.CheckAndCreateFile(filename);
    cout << "已打开/创建库:" << filename << endl;

    cout << "---加载内容中---" << endl;
    vector<Device> Data = ps.LoadData(filename);
    cout << "---加载成功---" << endl;

    cout << "---创建新设备---" << endl;
    Device device;
    device.id = 0; //这个会自动生成不用设置
    cout << "请输入设备名字：" << endl;
    cin >> device.name;
    cout << "请输入设备参数信息介绍：" << endl;
    cin >> device.params;
    cout << "请输入功能：" << endl;
    cin >> device.function;
    cout << "请输入功能码 [以空格分隔的十六进制数，0x01 0xEF]:" << endl;
    string inputfunc;
    cin.ignore();
    getline(cin, inputfunc);
    ReadVec(inputfunc, device.func_code);
    cout << "请输入响应信息 [以空格分隔的十六进制数]:" << endl;
    string inputres;
    getline(cin, inputres);
    ReadVec(inputres, device.response);

    ps.AddDevice(Data, device, filename);
    cout << "---已添加数据---" << endl;
    ps.PrintDevices(Data);

}

void User::ReadVec(const string &input, vector<int> &data) {

    istringstream iss(input);
    string hexValue;


    data.clear();
    //空格读取
    while (iss >> hexValue) {
        int value;
        stringstream ss;

        if (hexValue.find("0x") == 0) {
            ss << hex << hexValue.substr(2);
        } else {
            ss << hex << hexValue;
        }


        ss >> value;
        if (ss.fail()) {
            cout << "解析错误：" << hexValue << endl; // 添加错误处理
            continue; // 跳过此值
        }

        data.push_back(static_cast<int>(value));
    }

}

void User::FileDirectory() {
    string directoryPath = "..\\SaveFile\\*.amdb";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(directoryPath.c_str(), &findFileData);
    //不知道为什么查找不能使用filesystem库，这里用windows的api了
    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "当前没有数据库" << endl;
        return;
    }
    // cout << "已有数据库：" << endl;

    do {
        // 检查是否是常规文件
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            cout << findFileData.cFileName << endl;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}


