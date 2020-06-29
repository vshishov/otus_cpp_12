#include "server.h"

#include <iostream>

int main(int argc, char** argv)
{
  std::cout << argv[0]  << ' ' << argc << std::endl;

  try {
    boost::asio::io_service ios;
    otus::Server server(ios, 8080);
    ios.run();
  } catch(const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
