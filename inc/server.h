#pragma once

#include "session.h"

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace otus {

using boost::asio::ip::tcp;

class Server {
public:
  Server(boost::asio::io_service& ios, short port);

  void handle_accept(std::shared_ptr<Session> session,
                     const boost::system::error_code& err);

private:
  boost::asio::io_service& ios;
  tcp::acceptor acceptor;
};

} // otus::
