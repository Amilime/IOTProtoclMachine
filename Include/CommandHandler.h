//
// Created by AM on 4/1/2025.
//

#ifndef C_SERIAL_COMMAND_H
#define C_SERIAL_COMMAND_H

#include <string>
#include <unordered_map>
#include <vector>

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

    CommandType parseCommand(const std::string&command);

    void executeCommand(const CommandType& commandType, const std::string& command);

private:
    // 具体的命令实现
    void queryAll();
    void queryDb(const std::string& dbName);
    void queryId(const std::string& dbName, int id);
    void saveData(const std::string& dbName);
    void deleteData(const std::string& dbName,int id);
    void openSP();
    void openRC();
    void sendReq(const std::string& dbName, int id);
    void sendReqRes(const std::string& dbName, int id);

    // 提取参数辅助函数
    std::string extractDBName(const std::string& command);
    int extractID(const std::string& command);

};


#endif //C_SERIAL_COMMAND_H
