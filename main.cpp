#include <iostream>
#include <vector>
#include <iomanip>

#include "./Include/SerialPort.h"
#include "./Include/User.h"

using namespace std;


int main() {
    User user;
   // user.SendDemo("COM1",9600,0,8,1);
    //getchar();
    SerialPort port = user.Open("COM1",9600,0,8,1,0);

    user.SendDemo(port);

    user.ReceiveDemo(port);
    return 0;
}
