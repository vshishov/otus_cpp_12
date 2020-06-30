#pragma once

#include "session.h"
#include "commander.h"
#include "executer.h"
#include "logger.h"

#include <iostream>
#include <memory>
#include <boost/asio.hpp>

namespace otus {

using boost::asio::ip::tcp;

class Server {
public:
  Server(boost::asio::io_service& a_ioService, const tcp::endpoint& a_endPoint, std::size_t a_szBulkSize);

private:  
  void DoAccept();

private:
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  
  std::shared_ptr<Commander> m_pCommander;
  std::shared_ptr<Executer> m_pExecuter;
  std::shared_ptr<Logger> m_pLogger;
};

} // otus::
