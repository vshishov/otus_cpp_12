#include "session.h"

namespace otus {

Session::Session(boost::asio::io_service& ios)
  : socket(ios) 
{ }

tcp::socket& Session::get_socket() {
  return socket;
}

void Session::start() {
  socket.async_read_some(
      boost::asio::buffer(data),
      boost::bind(&Session::handle_read, this,
                  shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(std::shared_ptr<Session>& /*s*/,
                  const boost::system::error_code& err,
                  size_t bytes_transferred)
{
  if (!err) {
    std::cout << "recv: " << std::endl;
    socket.async_read_some(
        boost::asio::buffer(data),
        boost::bind(&Session::handle_read, this,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    std::cout.write(data, bytes_transferred);
    std::cout << std::endl;
  } else {
    std::cerr << "err (recv): " << err.message() << std::endl;
  }
}

} // otus::