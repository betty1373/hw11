#ifndef CmdReader_H
#define CmdReader_H
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <mutex>
#include "Database.h"

using cmd_t = std::string;
/// @file
/// @brief Class for processing commands.
/// @author btv<example@example.com>
class CmdReader {
public:
/// @brief Create object class - PatternCreater
    static std::shared_ptr<CmdReader> Create();
    ~CmdReader();
/// @brief Processes cmds and get response from database
    void NewCmd(const std::string& clientId, cmd_t& cmd);
/// @brief Add client to set
    void AddClient(const std::string& client);
/// @brief Delete client from set
    void DeleteClient(const std::string& client);
    cmd_t GetResponse(const std::string& client);
private:
    CmdReader();
/// @brief Get client's cmds
    std::map<std::string,cmd_t>::iterator GetContext(const std::string& clientId);
/// @brief Add client's cmds
    std::map<std::string,cmd_t>::iterator AddContext(const std::string& clientId);

    std::map<std::string,cmd_t> m_contexts;
    std::set<std::string> m_clients;
    std::unique_ptr<Database> m_database;
    std::mutex m_mutex;
};
#endif