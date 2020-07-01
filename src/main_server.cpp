#include "server.h"

#include <iostream>

using boost::asio::ip::tcp;

class InvalidInteger : public std::logic_error
{
public:
  InvalidInteger(const std::string& a_message)
    : logic_error(a_message)
  { }
};

long GetIntFromArgv(const char* argv)
{
  long lValue{0};
  try {
    lValue = std::stoi(argv);
  }
  catch (const std::invalid_argument& ex) {
    throw InvalidInteger("Number is incorrect");
  }
  catch (const std::out_of_range& ex) {
    throw InvalidInteger("Number is out of range");
  }

  if (lValue == 0) {
    throw InvalidInteger("Number must be greater then zero");
  }

  if (lValue < 0) {
    throw InvalidInteger("Number must not be negative");
  }

  return lValue;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <port> <bulk_size>" << std::endl;
    return 1;
  }

  try {
    int nPort = GetIntFromArgv(argv[1]);
    int nBulkSize = GetIntFromArgv(argv[2]);
    boost::asio::io_service ioService;
    tcp::endpoint endpoint(tcp::v4(), nPort);
    boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
    otus::Server server(ioService, endpoint, nBulkSize);
    signals.async_wait([&ioService](const boost::system::error_code& error, int /*signal_number*/)
      {
        if (!error) {
          ioService.stop();
        }
      }
    );
    
    ioService.run();
  }
  catch(const InvalidInteger& ex) {
    std::cerr << "Port or bulk_size is invalid" << std::endl;
    std::cerr << "Error: "<< ex.what() << std::endl;
  } 
  catch(const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
