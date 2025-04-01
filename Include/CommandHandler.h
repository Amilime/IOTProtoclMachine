//
// Created by AM on 4/1/2025.
//

#ifndef C_SERIAL_COMMAND_H
#define C_SERIAL_COMMAND_H

#include <string>
#include <unordered_map>
#include <vector>
#include "SerialPort.h"
#include "User.h"

// 命令类型枚举
enum CommandType {
    QUERY_ALL,
    QUERY_DB,
    QUERY_ID,
    SAVE_DATA,
    DELETE_DATA,
    OPEN_SP,
    OPEN_RC,
    SEND_REQ,
    SEND_REQ_RES,
    UNKNOWN
};

class CommandHandler {
public:

    CommandHandler();

    static CommandType parseCommand(const std::string&command);

    void executeCommand(const CommandType& commandType, const std::string& command);

private:
    // 具体的命令实现
    SerialPort currentPort;
    User user;
    bool isSPOpened = false;
    static void queryAll();
    static void queryDb(const std::string& dbName);
    static void queryId(const std::string& dbName, int id);
    static void saveData(const std::string& dbName);
    static void deleteData(const std::string& dbName,int id);
    void openSP();
    void openRC();
    void sendReq(const std::string& dbName, int id);
    void sendReqRes(const std::string& dbName, int id);

    // 提取参数辅助函数
    static std::string extractDBName(const std::string& command);
    static int extractID(const std::string& command);

};


#endif //C_SERIAL_COMMAND_H
