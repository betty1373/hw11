#include "inc/Server.h"
#include "iostream"
#include <chrono>
#include <thread>
#include <future>
#include <memory>
/// @file
/// @brief Main function. Test for processing commands.
/// @author btv<example@example.com>

int main(int argc, const char** argv) 
{  
  if (argc < 1) {
    std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
    return 1;
  }

try {
    int nPort = std::stoi(argv[1]);
   
    boost::asio::io_context ioContext;
    tcp::endpoint endpoint(tcp::v4(), nPort);
    boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
    Server server(ioContext, endpoint);
    signals.async_wait(
      [&ioContext](const boost::system::error_code& error, int /*signal_number*/)
      {
        if (!error) {
          ioContext.stop();
        }
      }
    );
    
    ioContext.run();
  } 
  catch(const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  std::cout<<"GoodBye"<<std::endl;
  return 0;
}
