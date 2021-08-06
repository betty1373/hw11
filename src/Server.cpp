#include "../inc/Server.h"
Session::Session(tcp::socket socket,  std::shared_ptr<CmdReader>& reader)
    : m_socket(std::move(socket)),
      m_cmdReader(reader)
{
    auto endpoint = m_socket.remote_endpoint();
    m_clientId = endpoint.address().to_string() + ":"+ std::to_string(endpoint.port());     
}

void Session::Start()
{ 
    m_cmdReader->AddClient(m_clientId);
    Do_read();
}

void Session::Do_read()
{
    auto self(shared_from_this());
    boost::asio::async_read(m_socket,
                            boost::asio::buffer(m_data),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          m_strstream.write(m_data,length);
          if (ec == boost::asio::error::eof || ec==boost::asio::error::connection_reset) {
            
            CloseSession();
          }
          else {
           // std::cout << "receive " << length << "=" << std::string{m_data, length} << std::endl;
             Work();
          }
          if (!ec) {
            Do_read();
          }
        });
}
void Session::CloseSession() {
  //std::cout<<"Close Session "<<m_clientId<<std::endl;
  m_cmdReader->DeleteClient(m_clientId);
}
void Session::Work() 
{ 
      std::string cmd;
      m_strstream.seekp(0);
      while (!std::getline(m_strstream, cmd).eof() ) {
         if (cmd.length() > 0 && cmd[cmd.length()-1]=='\r') {
           cmd = cmd.substr(0,cmd.length()-1);
        }
        m_cmdReader->NewCmd(m_clientId,cmd);
      }
      m_strstream.clear();
      m_strstream.str("");
      m_strstream.write(cmd.c_str(),cmd.size());
}
Server::Server(boost::asio::io_context& io_context, const tcp::endpoint& port, std::size_t numCmds)
    : m_acceptor(io_context, port),
    m_socket(io_context)
  {
    m_cmdReader =  CmdReader::Create(static_cast<size_t>(numCmds));
    m_consoleLogger = ConsoleLogger::Create("log",m_cmdReader);
    m_fileLogger = FileLogger::Create("file",m_cmdReader); 
    Do_accept();
  }

void Server::Do_accept()
{
    m_acceptor.async_accept(m_socket,
        [this](boost::system::error_code ec)
        {
          if (!ec)
          {
            std::make_shared<Session>(std::move(m_socket), m_cmdReader)->Start();
          }

          Do_accept();
        });
}