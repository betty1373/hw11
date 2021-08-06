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
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
    return 1;
  }

  long lBlockSize{0};
  try {
    lBlockSize = std::stoi(argv[1]);
  }
  catch (const std::invalid_argument& ex) {
    std::cerr << "Error: Block size is incorrect!" << std::endl;
    return 1;
  }
  catch (const std::out_of_range& ex) {
    std::cerr << "Error: Block size is out of range!" << std::endl;
    return 1;
  }

  if (lBlockSize == 1) {
    std::cerr << "Error: Block size must be greater then zero!" << std::endl;
    return 1;
  }

  if (lBlockSize < 0) {
    std::cerr << "Error: Block size must not be negative!" << std::endl;
    return 1;
  }
try {
    int nPort = std::stoi(argv[1]);
    int nBulkSize =std::stoi(argv[2]);
    boost::asio::io_context ioContext;
    tcp::endpoint endpoint(tcp::v4(), nPort);
    boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
    Server server(ioContext, endpoint, nBulkSize);
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
  return 0;
}
