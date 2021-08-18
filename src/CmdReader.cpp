#include "../inc/CmdReader.h"
#include <tuple>
#include <boost/algorithm/string.hpp>

std::shared_ptr<CmdReader> CmdReader::Create() {
    auto ptr = std::shared_ptr<CmdReader>{ new CmdReader{}};   
    return ptr;
}
 CmdReader::~CmdReader() {
    //m_cv.notify_all();
}
    
void CmdReader::NewCmd(const std::string& clientId, cmd_t& cmd)
{ 
    std::vector<std::string> parts;
    boost::trim(cmd);
    boost::split(parts, cmd, boost::is_any_of(" "));
    std::string result = "OK\n";

    auto command_match = [&](cmd_t cmd, size_t count)
    {
        boost::to_upper(parts[0]);
        return (parts[0] == cmd) && (parts.size() == count);
    };

    if (parts.size())
    {
        if(command_match("INSERT", 4))
        {
           
            auto err = m_database->Insert(parts[1], std::stoi(parts[2]), parts[3]);
            result = m_database->Db_Err(err, parts[2]);
        }
        else if (command_match("TRUNCATE", 2))
        {
           
            auto err = m_database->Truncate(parts[1]);
            result = m_database->Db_Err(err, parts[1]);
        }
        else if (command_match("INTERSECTION", 1))  {
            auto res = m_database->Intersection();
            result = res + result;
        }
        else if (command_match("SYMMETRIC_DIFFERENCE", 1))
        {
            auto res = m_database->Symmetric_difference();
            result = res + result;
        }
        else
        {          
            result = "ERR unknown command\n";
        }
    }
    else {
        result = "ERR bad command format\n";
    }
   
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        auto context = GetContext(clientId);
            
        context->second = result; 
    }
}

void CmdReader::AddClient(const std::string& client) {
    m_clients.emplace(client);
}
void CmdReader::DeleteClient(const std::string& client) {
    m_clients.erase(client);
}
cmd_t CmdReader::GetResponse(const std::string& client) {
    auto it = GetContext(client);
    auto& context = it->second; 
    auto cmds = context;
    context.resize(0);   
    return cmds;
}   
CmdReader::CmdReader() 
{
    m_database = std::make_unique<Database>();
    m_database->AddTable("A");
    m_database->AddTable("B");
     //m_cmds.reserve(m_num_cmds);
}

// std::stringstream CmdReader::FormBatch(std::vector<std::string>& cmds) {
    
//     std::stringstream ss;
    
//     for (auto it_cmd = cmds.cbegin();it_cmd!=cmds.cend();it_cmd++) {
//         if (it_cmd !=cmds.cbegin())
//             ss<< ", "<<*it_cmd;
//         else
//             ss<<"bulk: "<<*it_cmd;
//     }
//     return ss;     
// }
std::map<std::string,cmd_t>::iterator CmdReader::GetContext(const std::string& clientId) {
    
    auto it = m_contexts.find(clientId);
    
    if (it==m_contexts.end()) {
        it = AddContext("main");
    }
    return it;
}
std::map<std::string,cmd_t>::iterator CmdReader::AddContext(const std::string& clientId) {

    auto it = m_contexts.find(clientId);
    
    if (it==m_contexts.end()) {
        auto [insIt, inserted] = m_contexts.try_emplace(clientId);
        if (inserted) it = insIt;
    }
    return it;
}
