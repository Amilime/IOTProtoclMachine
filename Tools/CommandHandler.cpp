//
// Created by Am on 4/1/2025.
//

#include "../Include/CommandHandler.h"
#include "../Include/User.h"
#include "../Include/SerialPort.h"
#include "../Include/ProtocolSave.h"

#include <iostream>
#include <sstream>

using namespace std;

CommandHandler::CommandHandler() {
    
}

// 命令解析
CommandType CommandHandler::parseCommand(const string &command) {
    // 解析命令字符串并返回对应的 CommandType
    if (command == "QUERY ALL") {
        return QUERY_ALL;
    } else if (command.find("QUERY") == 0) {
        if (command.find("WHERE ID=") != string::npos) {
            return QUERY_ID;
        } else {
            return QUERY_DB;
        }
    } else if (command.find("SAVEDATA TO") == 0) {
        return SAVE_DATA;
    } else if (command == "OPEN SP") {
        return OPEN_SP;
    } else if (command == "OPEN RC") {
        return OPEN_RC;
    } else if (command.find("SEND REQ FROM") == 0) {
        return SEND_REQ;
    } else if (command.find("SEND REQ&RES FROM") == 0) {
        return SEND_REQ_RES;
    }
    return UNKNOWN;

}

// 执行命令
void CommandHandler::executeCommand(const CommandType& commandType, const std::string& command) {
    switch (commandType) {
        case QUERY_ALL:
            queryAll();
            break;
        case QUERY_DB: {
            string dbName = extractDBName(command);
            queryDb(dbName);
            break;
        }
        case QUERY_ID: {
            std::string dbName = extractDBName(command);
            int id = extractID(command);
            queryId(dbName, id);
            break;
        }
        case SAVE_DATA: {
            std::string dbName = extractDBName(command);
            saveData(dbName);
            break;
        }
        case OPEN_SP:
            openSP();
            break;
        case OPEN_RC:
            openRC();
            break;
        case SEND_REQ: {
            std::string dbName = extractDBName(command);
            int id = extractID(command);
            sendReq(dbName, id);
            break;
        }
        case SEND_REQ_RES: {
            std::string dbName = extractDBName(command);
            int id = extractID(command);
            sendReqRes(dbName, id);
            break;
        }
        default:
            std::cerr << "未知命令！" << std::endl;
            break;
    }
}

// 辅助函数：提取 DBNAME
std::string CommandHandler::extractDBName(const std::string& command) {
    size_t start = command.find("{");
    size_t end = command.find("}");
    if (start != string::npos && end != string::npos && start < end) {
        return command.substr(start + 1, end - start - 1); // 提取并返回 DBNAME
    }
    return " ";
}

// 辅助函数：提取 ID
int CommandHandler::extractID(const std::string& command) {
    size_t pos = command.find("ID=");
    if (pos != std::string::npos) {
        size_t id_start = pos + 3; // 跳过 "ID="
        // 继续查找接下来的空格或字符串结尾
        size_t id_end = command.find_first_of(" }", id_start); // 查找最近的空格或 '}'
        if (id_end != std::string::npos) {
            std::string id_str = command.substr(id_start, id_end - id_start); // 提取 ID 部分
            try {
                return std::stoi(id_str); // 转换为整型
            } catch (std::invalid_argument&) {
                std::cerr << "无效的 ID 值: " << id_str << std::endl;
                return -1; // 返回无效标志
            } catch (std::out_of_range&) {
                std::cerr << "ID 值超出范围: " << id_str << std::endl;
                return -1; // 返回无效标志
            }
        }
    }
    return -1; // 未找到 ID 返回 -1
}

// 具体命令的实现
void CommandHandler::queryAll() {
    std::cout << "查询所有数据库的功能.\n";
}

void CommandHandler::queryDb(const std::string& dbName) {
    std::cout << "查询数据库: " << dbName << "\n";
}

void CommandHandler::queryId(const std::string& dbName, int id) {
    std::cout << "查询数据库: " << dbName << " 中的 ID: " << id << "\n";
}

void CommandHandler::saveData(const std::string& dbName) {
    std::cout << "保存数据到数据库: " << dbName << "\n";
}

void CommandHandler::openSP() {
    std::cout << "开启串口通信.\n";
}

void CommandHandler::openRC() {
    std::cout << "开启接收通道.\n";
}

void CommandHandler::sendReq(const std::string& dbName, int id) {
    std::cout << "发送请求，数据库: " << dbName << " ID: " << id << "\n";
}

void CommandHandler::sendReqRes(const std::string& dbName, int id) {
    std::cout << "发送请求和响应，数据库: " << dbName << " ID: " << id << "\n";
}