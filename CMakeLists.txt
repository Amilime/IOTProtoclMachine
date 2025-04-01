cmake_minimum_required(VERSION 3.23)
project(C_Serial)

set(CMAKE_CXX_STANDARD 14)

add_executable(C_Serial main.cpp Tools/SerialPort.cpp Include/SerialPort.h Tools/ProtocolSave.cpp Include/ProtocolSave.h Tools/ProtocolLoad.cpp Include/ProtocolLoad.h Tools/User.cpp Include/User.h Test/test1.cpp Test/test1.h)
