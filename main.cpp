#include <iostream>
#include "./Include/SerialPort.h"

using namespace std;

void sendDemo(){
    SerialPort S1;
    if(S1.openSerial("COM1",9600,0,8,1)){
        for (int i = 0; i < 10; ++i) {
            S1.sendData("helloworld",10);
        }
        cout <<"send demo finished"<<endl;
    } else{
        cout<<"open serial port failed"<<endl;
    }
}

void receiveDemo(){
    SerialPort S1;
    if(S1.openSerial("COM1",9600,0,8,1)){
       char buf[4096];
       while(true){
           memset(buf,0,4096);
           S1.receiveData(buf,4096);
           cout<<buf<<' ';
       }
    }
}

int main() {
    sendDemo();
    //getchar();
    return 0;
}
