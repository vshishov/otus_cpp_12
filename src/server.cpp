#include "server.h"

namespace otus {

Server::Server(boost::asio::io_service& a_ioService, const tcp::endpoint& a_endPoint, std::size_t a_szBulkSize)
  : m_acceptor(a_ioService, a_endPoint)
  , m_socket(a_ioService)
{
  m_pCommander = otus::Commander::Create("main", a_szBulkSize);
  m_pExecuter = otus::Executer::Create("log", m_pCommander);
  m_pLogger = otus::Logger::Create("file", m_pCommander);

  DoAccept();
}

void Server::DoAccept()
{
  m_acceptor.async_accept(m_socket,
    [this](boost::system::error_code ec)
    {
      if (!ec) {
        std::make_shared<Session>(std::move(m_socket), m_pCommander)->Start();
      }

      DoAccept();
    });
}

} // otus::
