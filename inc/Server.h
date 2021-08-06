#ifndef Server_H
#define Server_H
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "Logger.h"

using boost::asio::ip::tcp;
/// @file
/// @brief Class for clients's session
/// @author btv<example@example.com>
class Session  : public std::enable_shared_from_this<Session>
{
public:
/// @brief Session constructor
  Session(tcp::socket socket,std::shared_ptr<CmdReader>& reader);
/// @brief Start session
  void Start();
private:
/// @brief Receive data from client
  void Do_read();
/// @brief Close clients's session
  void CloseSession();
/// @brief Process data, receiving from client
  void Work();

  tcp::socket m_socket;
  std::shared_ptr<CmdReader> m_cmdReader;
  //enum { max_length = 1024 };
  char m_data[1];
  std::string m_clientId;
  std::stringstream m_strstream;
};
/// @file
/// @brief Class for server
/// @author btv<example@example.com>
class Server
{
public:
/// @brief Serevr constructor
  Server(boost::asio::io_context& io_context,  const tcp::endpoint& port, std::size_t numCmds); 
private:
/// @brief Process of server
  void Do_accept();

  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  std::shared_ptr<CmdReader> m_cmdReader;
  std::shared_ptr<ConsoleLogger> m_consoleLogger;
  std::shared_ptr<FileLogger> m_fileLogger;
};

#endif