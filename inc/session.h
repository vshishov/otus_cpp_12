#pragma once

#include "commander.h"

#include <iostream>
#include <memory>
#include <boost/asio.hpp>

namespace otus {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(tcp::socket a_Socket, std::shared_ptr<Commander> a_pCommander);
  
  void Start();

private:
  void DoRead();
  void Proccess();
  void Flush();

private:
  tcp::socket m_Socket;
  std::shared_ptr<Commander> m_pCommander;
  char m_buffer[1];
  std::stringstream m_ssInputStream;
  std::string m_strId;
};

} // otus::
