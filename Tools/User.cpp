//
// Created by y on 3/30/2025.
//

#include "../Include/User.h"
#include "../Include/ProtocolSave.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>


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

void User::SendDemo(SerialPort &S) {

    const int func[]{0x01}; //功能码池
    int start = 0x24;
    vector<int> data{0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0X0B, 0xFE};
    int length = 0x08;


    S.sendData(&start, 1);
    S.sendData(&func[0], 1);
    S.sendData(&length, 1);
    cout << "send: " << endl;
    for (int &i: data) {
        cout << hex << ' ' << i;
        S.sendData(&i, 1);

        // cout <<"send demo finished"<<endl;
    }
    cout << endl;

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

            cout<<"输入串口名[windows以COM+数字起名]: "<<endl;
            cin >> portname;
            cout<<"输入波特率[4800,9600,115200 ...]: "<<endl;
            cin >> baudrate;
            cout<<"校验位  [0:无校验, 1:奇校验, 2:偶校验, 3:标记校验]:"<<endl;
            cin >> parity;

            cout<<"数据位 [ 4 - 8, 一般填 8]: "<<endl;
            cin >> databit;

            cout<<"停止位  [ 1:一位, 2:两位, 3:一点五位]: "<<endl;
            cin >>stopbit;

            cout<<"同步或异步 [同步1, 异步0]:"<<endl;
            cin>> synchronizeflag;


        } else {
            cout <<"错误的输入值，请输入 'y' 或 'n'" <<endl;
            continue;
        }

        try {
            parity = static_cast<char>(parity);
            databit = static_cast<char>(databit);
            stopbit = static_cast<char>(stopbit);
            synchronizeflag = static_cast<char>(synchronizeflag);
            SerialPort S = user.Open(portname.c_str(), baudrate, parity, databit, stopbit, synchronizeflag);
            cout<<"串口开启成功:"<<portname<<endl;
            res = true;
            return S;

        }catch (const runtime_error &e) {
            cout<<e.what() <<endl;
            cout << "请重新输入参数"<<endl;
        }

    }
}

// 数据存储
void User::SaveData() {
    const string locate = "../SaveFile";
    string filename;
    cout<<"输入你要存储/创建的库:"<<endl;
    cin>> filename;
    string FinalName = locate+filename+".amdb";
    ProtocolSave ps;
    ps.CheckAndCreateFile(FinalName);
    cout<<"已打开/创建库:"<<FinalName<<endl;

    cout<<"---加载内容中---"<<endl;
    vector<Device> Data = ps.LoadData(FinalName);
    cout<<"---加载成功---"<<endl;



}
