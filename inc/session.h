#pragma once

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace otus {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(boost::asio::io_service& ios);
  
  tcp::socket& get_socket();
  void start();
  void handle_read(std::shared_ptr<Session>& s,
                   const boost::system::error_code& err,
                   size_t bytes_transferred);

private:
  tcp::socket socket;
  char data[1024];
};

} // otus::
