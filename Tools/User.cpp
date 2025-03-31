//
// Created by y on 3/30/2025.
//

#include "../Include/User.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

User::User() {

}

SerialPort User::Open(const char *portname,
                      int baudrate,
                      char parity,
                      char databit,
                      char stopbit,
                      char synchronizeflag){

    SerialPort S;
    // 实际使用参数
    int State = S.openSerial(portname, baudrate, parity, databit, stopbit,synchronizeflag);

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
    vector<int> data{0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0X0B,0xFE};
    int length = 0x08;


        S.sendData(&start,1);
        S.sendData(&func[0],1);
        S.sendData(&length,1);
        for (int & i : data) {
            cout<<"send: "<<hex<< i<<endl;
            S.sendData(&i,1);

        cout <<"send demo finished"<<endl;
    }

}

void User::ReceiveDemo(SerialPort &S){




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


