#ifndef Logger_H
#define Logger_H
#include <fstream>
#include <atomic>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "CmdReader.h"
/// @file
/// @brief Class outs data from stringstream to console
/// @author btv<example@example.com>
class ConsoleLogger : public Observer, public std::enable_shared_from_this<ConsoleLogger> {
public:
/// @brief Create object class - PatternCreater
     static std::shared_ptr<ConsoleLogger> Create(const std::string& name, std::shared_ptr<CmdReader>& reader); 
     ~ConsoleLogger(); 
/// @brief Puts data from stringstream to queue
    void Update(std::stringstream& ss) override;
private:
    ConsoleLogger(const std::string& m_name);
/// @brief Subscribe to receiving notifications
    void SetCmdReader(std::shared_ptr<CmdReader>& _reader);
/// @brief Gets data from queue and out to console
    void Work();
/// @brief Stop thread
    void StopWork();

    std::weak_ptr<CmdReader> m_reader;   
    std::mutex m_mutex;
    std::mutex m_outmutex;    
    std::atomic<bool> m_stop;
    std::thread m_thread;
    std::condition_variable m_cv;
    std::queue<std::string> m_deque;
};
/// @file
/// @brief Class outs data from stringstream to file
/// @author btv<example@example.com>
class FileLogger : public Observer, public std::enable_shared_from_this<FileLogger> {
public:  
/// @brief Create object class - PatternCreater  
     static std::shared_ptr<FileLogger> Create(const std::string& name,std::shared_ptr<CmdReader>& reader);
     ~FileLogger();
/// @brief Puts data from stringstream to queue
    void Update(std::stringstream& ss) override;

private:
    FileLogger(const std::string& name);
/// @brief Gets data from queue and out to file
    void Work(std::string postfix);
/// @brief Stop thread
    void StopWork();
/// @brief Subscribe to receiving notifications
    void SetCmdReader(std::shared_ptr<CmdReader>& _reader);

    std::weak_ptr<CmdReader> m_reader;
     std::atomic<bool> m_stop;
    std::vector<std::thread> m_threads;
    std::mutex m_mutex;   
    std::condition_variable m_cv;
    std::queue<std::string> m_deque;
};
#endif