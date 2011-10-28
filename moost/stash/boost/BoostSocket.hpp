#ifndef BOOSTSOCKET_HPP
#define BOOSTSOCKET_HPP

#include <boost/bind.hpp>

#include "moost/stash/boost/BoostSocket.h"

namespace moost {
namespace stash {

template<typename Protocol>
BoostSocket<Protocol>::BoostSocket(BoostDispatcher &dispatcher)
    : _handle(dispatcher._ioserv)
{
}

template<typename Protocol>
BoostSocket<Protocol>::BoostSocket(BoostDispatcher &dispatcher,
                                   BoostIP<Protocol> const &protocol)
    : _handle(dispatcher._ioserv, protocol._handle)
{
}

template<typename Protocol>
BoostSocket<Protocol>::BoostSocket(BoostDispatcher &dispatcher,
                                   BoostEndpoint<Protocol> const &ep)
    : _handle(dispatcher._ioserv, ep)
{
}

template<typename Protocol>
BoostSocket<Protocol>::~BoostSocket()
{
}

template<typename Protocol>
void BoostSocket<Protocol>::open(BoostIP<Protocol> const &protocol, Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.open(protocol._handle, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
template<typename ConnectHandler>
void BoostSocket<Protocol>::async_connect(BoostEndpoint<Protocol> const &ep, ConnectHandler cb)
{
  _handle.async_connect(ep, boost::bind(BoostSocket<Protocol>::_async_connect_handle<ConnectHandler>, cb, boost::asio::placeholders::error));
}

template<typename Protocol>
void BoostSocket<Protocol>::bind(BoostEndpoint<Protocol> const &ep, Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.bind(ep, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocket<Protocol>::close(Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.close(berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocket<Protocol>::connect(BoostEndpoint<Protocol> const &ep, Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.connect(ep, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocket<Protocol>::cancel(Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.cancel(berr);
  boost2moost_error(err, berr);
}


} // stash
} // moost

#endif /* !BOOSTSOCKET_HPP */
