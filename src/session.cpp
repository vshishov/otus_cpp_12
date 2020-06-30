#include "session.h"

namespace otus {

Session::Session(tcp::socket a_Socket, std::shared_ptr<Commander> a_pCommander)
  : m_Socket(std::move(a_Socket)) 
  , m_pCommander(a_pCommander)
{ 
  auto ep = m_Socket.remote_endpoint();
  m_strId = ep.address().to_string() + ":" + std::to_string(ep.port());
}

void Session::Start() 
{
  DoRead();
}

void Session::DoRead()
{
  auto self(shared_from_this());
  boost::asio::async_read(m_Socket,
    boost::asio::buffer(m_buffer),
    [this, self](boost::system::error_code ec, std::size_t length)
    {
      // std::cout << "Session::DoRead, this==" << this << ", ec=" << ec << ", mReadMsg=" << &m_buffer[0] << ", length=" << length << std::endl;
      m_ssInputStream.write(m_buffer, length);
      if(ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
        Flush();
      } 
      else {
      
      Proccess();
      }
      
      if (!ec) {
        DoRead();
      }
    }
  );
}

void Session::Proccess()
{
  // m_ssInputStream.seekp(0);
  // std::string strLine;
  // while ( std::getline(m_ssInputStream, strLine) ) {
  //   if (strLine.length() > 0 && strLine[strLine.length() - 1] == '\r') {
  //     strLine = strLine.substr(0, strLine.length() - 1);
  //   }
  //   // text.push_back(line);
  //   m_pCommander->ProccessLine(strLine);
  // }
  // m_ssInputStream.clear();
  // m_ssInputStream.str("");
  // m_ssInputStream << strLine;

  // std::string strLine;
  // while ( std::getline(m_ssInputStream, strLine) ) {
  //   m_pCommander->ProcessLine(strLine);
  // }
  // m_ssInputStream.clear();
  // m_ssInputStream.str("");



  std::string line;
  m_ssInputStream.seekp(0);
  while (!std::getline(m_ssInputStream, line).eof())
  {
    if (line.length() > 0 && line[line.length() - 1] == '\r') {
      line = line.substr(0, line.length() - 1);
    }
    m_pCommander->ProcessLine(m_strId,line);
  }
  m_ssInputStream.clear();
  m_ssInputStream.str("");
  m_ssInputStream.write(line.c_str(), line.size());

}

void Session::Flush()
{
  std::cout << "Flush" << std::endl;
  m_pCommander->Stop(m_strId);
}


} // otus::