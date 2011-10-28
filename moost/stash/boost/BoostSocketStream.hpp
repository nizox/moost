#ifndef BOOSTSOCKETSTREAM_HPP
#define BOOSTSOCKETSTREAM_HPP

#include "moost/stash/boost/BoostSocketStream.h"

namespace moost {
namespace stash {

template<typename Protocol>
BoostSocketStream<Protocol>::BoostSocketStream(BoostDispatcher &dispatcher)
    :  BoostSocket<Protocol>(dispatcher)
{
}

template<typename Protocol>
BoostSocketStream<Protocol>::BoostSocketStream(BoostDispatcher &dispatcher,
                                               BoostIP<Protocol> const &protocol)
    :  BoostSocket<Protocol>(dispatcher, protocol)
{
}

template<typename Protocol>
BoostSocketStream<Protocol>::BoostSocketStream(BoostDispatcher &dispatcher,
                                               BoostEndpoint<Protocol> const &ep)
    :  BoostSocket<Protocol>(dispatcher, ep)
{
}

template<typename Protocol>
BoostSocketStream<Protocol>::~BoostSocketStream()
{
}


template<typename Protocol>
template<typename ReceiveHandler>
void BoostSocketStream<Protocol>::async_receive(Buffer &buf, ReceiveHandler cb)
{
  BoostSocket<Protocol>::_handle.async_receive(boost_buffer(buf),
      boost::bind(BoostSocketStream<Protocol>::_async_handle<ReceiveHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
template<typename ReceiveHandler>
void BoostSocketStream<Protocol>::async_receive_until(moost::streambuf &buf, std::string const &stop, ReceiveHandler cb)
{
}

template<typename Protocol>
template<typename SendHandler>
void BoostSocketStream<Protocol>::async_send(Buffer const &buf, SendHandler cb)
{
  BoostSocket<Protocol>::_handle.async_send(boost_buffer(buf),
      boost::bind(BoostSocketStream<Protocol>::_async_handle<SendHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
size_t BoostSocketStream<Protocol>::receive(Buffer &buf, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.receive(boost_buffer(buf), 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

template<typename Protocol>
size_t BoostSocketStream<Protocol>::send(Buffer const &buf, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.send(boost_buffer(buf), 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

} // stash
} // moost

#endif /* !BOOSTSOCKETSTREAM_HPP */
