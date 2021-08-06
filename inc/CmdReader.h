#ifndef CmdReader_H
#define CmdReader_H
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "Observer.h"
/// @file
/// @brief Structure for save cmds
/// @author btv<example@example.com>
struct CmdBlk
{
    std::size_t m_cnt_braces;
    std::vector<std::string> m_cmds;
    CmdBlk()=default;
};

/// @file
/// @brief Class for processing commands and notifying subscribers.
/// @author btv<example@example.com>
class CmdReader : public Observable {
public:
/// @brief Create object class - PatternCreater
    static std::shared_ptr<CmdReader> Create(size_t num_cmds,std::istream& istream=std::cin);
    ~CmdReader();
/// @brief Adds subscribers to list
    void Subscribe(const std::shared_ptr<Observer>& obs) override;    
/// @brief Notifies subscribers and sends batch of cmds to their   
    void Notify(std::vector<std::string>& cmds) override;
   
/// @brief Processes cmds and inform subscribers
    void NewCmd(const std::string& clientId, const std::string& cmd);
/// @brief Add client to set
    void AddClient(const std::string& client);
/// @brief Delete client from set
    void DeleteClient(const std::string& client);
private:
/// @brief Private Constructor
    CmdReader(size_t num_cmds,std::istream& istream=std::cin);
/// @brief Forms batch to stringstream
    std::stringstream FormBatch(std::vector<std::string>& cmds);
/// @brief Get client's cmds
    std::map<std::string,CmdBlk>::iterator GetContext(const std::string& clientId);
/// @brief Add client's cmds
    std::map<std::string,CmdBlk>::iterator AddContext(const std::string& clientId);

/// @brief Push cmds to loggers
    void CmdLog(bool to_log=false);

    std::map<std::string,CmdBlk> m_contexts;
    std::set<std::string> m_clients;

    std::mutex m_mutex;
    std::condition_variable m_cv;

    size_t m_num_cmds;
    std::istream& m_istream;
   // size_t m_cnt_braces;
    //std::vector<std::string> m_cmds;
    std::list<std::weak_ptr<Observer>> m_observers;
};
#endif