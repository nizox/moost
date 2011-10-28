#ifndef BOOSTSOCKETDATAGRAM_HPP
#define BOOSTSOCKETDATAGRAM_HPP

#include "moost/stash/boost/BoostSocketDatagram.h"

#include <boost/bind.hpp>

namespace moost {
namespace stash {

template<typename Protocol>
BoostSocketDatagram<Protocol>::BoostSocketDatagram(BoostDispatcher &dispatcher)
    : BoostSocket<Protocol>(dispatcher) 
{
}

template<typename Protocol>
BoostSocketDatagram<Protocol>::BoostSocketDatagram(BoostDispatcher &dispatcher,
                                                   BoostIP<Protocol> const &protocol)
    : BoostSocket<Protocol>(dispatcher, protocol)
{
}

template<typename Protocol>
BoostSocketDatagram<Protocol>::BoostSocketDatagram(BoostDispatcher &dispatcher,
                                                   BoostEndpoint<Protocol> const &ep)
    : BoostSocket<Protocol>(dispatcher, ep)
{
}

template<typename Protocol>
BoostSocketDatagram<Protocol>::~BoostSocketDatagram()
{
}

template<typename Protocol>
template<typename ReceiveHandler>
void BoostSocketDatagram<Protocol>::async_receive(Buffer &buf, ReceiveHandler cb)
{
  BoostSocket<Protocol>::_handle.async_receive(boost_buffer(buf),
      boost::bind(BoostSocketDatagram<Protocol>::_async_handle<ReceiveHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
template<typename ReceiveHandler>
void BoostSocketDatagram<Protocol>::async_receive_from(Buffer &buf, BoostEndpoint<Protocol> &ep, ReceiveHandler cb)
{
  BoostSocket<Protocol>::_handle.async_receive_from(boost_buffer(buf), ep,
      boost::bind(BoostSocketDatagram<Protocol>::_async_handle<ReceiveHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
template<typename SendHandler>
void BoostSocketDatagram<Protocol>::async_send(Buffer const &buf, SendHandler cb)
{
  BoostSocket<Protocol>::_handle.async_send(boost_buffer(buf),
      boost::bind(BoostSocketDatagram<Protocol>::_async_handle<SendHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
template<typename SendHandler>
void BoostSocketDatagram<Protocol>::async_send_to(Buffer const &buf, BoostEndpoint<Protocol> const &ep, SendHandler cb)
{
  BoostSocket<Protocol>::_handle.async_send_to(boost_buffer(buf), ep,
      boost::bind(BoostSocketDatagram<Protocol>::_async_handle<SendHandler>, cb, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

template<typename Protocol>
size_t BoostSocketDatagram<Protocol>::receive(Buffer &buf, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.receive(boost_buffer(buf), 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

template<typename Protocol>
size_t BoostSocketDatagram<Protocol>::receive_from(Buffer &buf, BoostEndpoint<Protocol> &ep, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.receive_from(boost_buffer(buf), ep, 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

template<typename Protocol>
size_t BoostSocketDatagram<Protocol>::send(Buffer const &buf, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.send(boost_buffer(buf), 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

template<typename Protocol>
size_t BoostSocketDatagram<Protocol>::send_to(Buffer const &buf, BoostEndpoint<Protocol> const &ep, Error &err)
{
  size_t bytes;
  boost::system::error_code berr;

  bytes = BoostSocket<Protocol>::_handle.send_to(boost_buffer(buf), ep, 0, berr);
  boost2moost_error(err, berr);
  return bytes;
}

} // stash
} // moost

#endif /* !BOOSTSOCKETDATAGRAM_HPP */
