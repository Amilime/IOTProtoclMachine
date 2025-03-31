#include <iostream>
#include <vector>
#include <iomanip>

#include "./Include/SerialPort.h"
#include "./Include/User.h"

using namespace std;


int main() {
    User user;
    SerialPort port = user.CreateSP();
    user.SendDemo(port);

    user.ReceiveDemo(port);
    return 0;
}
