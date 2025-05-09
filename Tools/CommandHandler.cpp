//
// Created by Am on 4/1/2025.
//

#include "../Include/CommandHandler.h"
#include "../Include/User.h"
#include "../Include/SerialPort.h"
#include "../Include/ProtocolSave.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

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
    } else if (command.find("DELETE") == 0) {
        return DELETE_DATA;
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
void CommandHandler::executeCommand(const CommandType &commandType, const std::string &command) {
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
            string dbName = extractDBName(command);
            int id = extractID(command);
            queryId(dbName, id);
            break;
        }
        case SAVE_DATA: {
            string dbName = extractDBName(command);
            saveData(dbName);
            break;
        }
        case DELETE_DATA: {
            string dbName = extractDBName(command);
            int id = extractID(command);
            deleteData(dbName, id);
        }
        case OPEN_SP:
            openSP();
            break;
        case OPEN_RC:
            openRC();
            break;
        case SEND_REQ: {
            string dbName = extractDBName(command);
            int id = extractID(command);
            sendReq(dbName, id);
            break;
        }
        case SEND_REQ_RES: {
            string dbName = extractDBName(command);
            int id = extractID(command);
            sendReqRes(dbName, id);
            break;
        }
        default:
            cerr << "未知命令！" << endl;
            break;
    }
}

// 辅助函数：提取 DBNAME
string CommandHandler::extractDBName(const std::string &command) {
    size_t start = command.find("{");
    size_t end = command.find("}");
    if (start != std::string::npos && end != std::string::npos && start < end) {
        return command.substr(start + 1, end - start - 1); // 提取并返回 DBNAME
    }
    return "";
}

// 提取 ID，处理为空格和大括号情况
int CommandHandler::extractID(const string &command) {
    size_t pos = command.find("ID=");
    if (pos != string::npos) {
        size_t id_start = pos + 3; // 跳过 "ID="

        // 跳过空格
        while (id_start < command.length() && command[id_start] == ' ') {
            id_start++;
        }

        // 查找 ID 的结束位置，跳过 `{` 和空格
        size_t id_end = command.find_first_of(" }", id_start);

        // 处理 `{` 引导
        if (command[id_start] == '{') {
            id_start++; // 跳过 `{`
        }

        // 查找 ID 的结束位置
        id_end = command.find_first_of("}", id_start);

        if (id_end != string::npos) {
            string id_str = command.substr(id_start, id_end - id_start);



            // 处理大括号，去除所有多余的字符
            id_str.erase(std::remove(id_str.begin(), id_str.end(), '{'), id_str.end());
            id_str.erase(std::remove(id_str.begin(), id_str.end(), '}'), id_str.end());

            // 尝试转换为整型
            try {
                return std::stoi(id_str); // 转换为整型
            } catch (std::invalid_argument &) {
                std::cerr << "无效的 ID 值: " << id_str << std::endl;
                return -1; // 返回无效标志
            } catch (std::out_of_range &) {
                std::cerr << "ID 值超出范围: " << id_str << std::endl;
                return -1; // 返回无效标志
            }
        }
    }
    return -1; // 未找到 ID 返回 -1
}

// 具体命令的实现
void CommandHandler::queryAll() {   // 查询所有的数据库
    User user;
    User::FileDirectory();
}

void CommandHandler::queryDb(const std::string &dbName) {  //查询某个数据库里的所有信息
    ProtocolSave ps;
    const string locate = "../SaveFile/";
    string filename = locate + dbName + ".amdb";
    vector<Device> Data = ProtocolSave::LoadData(filename);
    ProtocolSave::PrintDevices(Data);
}

void CommandHandler::queryId(const std::string &dbName, int id) {  //查询某个数据库里某个ID的所有信息
    std::cout << "暂未实现（似乎也没必要）" << dbName << " 中的 ID: " << id << "\n";
}

void CommandHandler::saveData(const std::string &dbName) { //存储数据
    User user;
    const string locate = "../SaveFile/";
    string filename = locate + dbName + ".amdb";
    User::SaveData(filename);
}

void CommandHandler::deleteData(const std::string &dbName, int id) {
    ProtocolSave ps;
    const string locate = "../SaveFile/";
    string filename = locate + dbName + ".amdb";
    vector<Device> devices_table = ProtocolSave::LoadData(filename);
    ProtocolSave::RemoveDevice(devices_table, id, filename);
    cout << "---删除完成---" << endl;
    ProtocolSave::PrintDevices(devices_table);
}

void CommandHandler::openSP() {
    cout << "开启串口通信\n";
    currentPort = User::CreateSP(); // 其实这里预设参数也应该存入数据库的，但是懒了暂时没做（
    isSPOpened = true;
}

void CommandHandler::openRC() {
    if (isSPOpened) {
        cout << "开启接收通道\n";
        user.StartReceiving(currentPort);
        // User::ReceiveDemo(currentPort);
    } else {
        cout << "串口未开启，无法接受数据\n";
    }
}

void CommandHandler::sendReq(const std::string &dbName, int id) {
    if (isSPOpened) {
        const string locate = "../SaveFile/";
        string filename = locate + dbName + ".amdb";
        User user;
        User::SendDemo(currentPort, filename, id);
    } else {
        cout << "串口未开启，无法接受数据\n";
    }
}

void CommandHandler::sendReqRes(const std::string &dbName, int id) {
    if (isSPOpened) {
        const string locate = "../SaveFile/";
        string filename = locate + dbName + ".amdb";
        User user;
        User::SendDemo2(currentPort, filename, id);
    } else {
        cout << "串口未开启，无法接受数据\n";
    }
}