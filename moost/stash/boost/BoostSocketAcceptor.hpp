#ifndef BOOSTSOCKETACCEPTOR_HPP
#define BOOSTSOCKETACCEPTOR_HPP

#include <boost/bind.hpp>

#include "moost/stash/boost/BoostSocketAcceptor.h"

namespace moost {
namespace stash {

template<typename Protocol>
BoostSocketAcceptor<Protocol>::BoostSocketAcceptor(BoostDispatcher &dispatcher)
    : _handle(dispatcher._ioserv)
{
}

template<typename Protocol>
BoostSocketAcceptor<Protocol>::BoostSocketAcceptor(BoostDispatcher &dispatcher,
                                                   BoostIP<Protocol> const &protocol)
    : _handle(dispatcher._ioserv, protocol._handle)
{
}

template<typename Protocol>
BoostSocketAcceptor<Protocol>::BoostSocketAcceptor(BoostDispatcher &dispatcher,
                                                   BoostEndpoint<Protocol> const &ep,
                                                   BoostSocketAcceptor<Protocol>::ReuseAddress reuse)
    : _handle(dispatcher._ioserv, ep, reuse.value())
{
}

template<typename Protocol>
BoostSocketAcceptor<Protocol>::~BoostSocketAcceptor()
{
}

template<typename Protocol>
template<typename Option>
Option BoostSocketAcceptor<Protocol>::get_option(Option opt, Error &err)
{
  Option ret;
  boost::system::error_code berr;

  ret = _handle.get_option(opt, berr);
  boost2moost_error(err, berr);
  return ret;
}

template<typename Protocol>
template<typename Option>
void BoostSocketAcceptor<Protocol>::set_option(Option opt, Error &err)
{
  boost::system::error_code berr;

  _handle.set_option(opt, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::open(BoostIP<Protocol> const &protocol, Error &err)
{
  boost::system::error_code berr;

  _handle.open(protocol._handle, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
template<typename AcceptHandler>
void BoostSocketAcceptor<Protocol>::async_accept(BoostSocket<Protocol> &sock, BoostEndpoint<Protocol> &ep, AcceptHandler cb)
{
  _handle.async_accept(sock._handle, ep, boost::bind(&BoostSocketAcceptor<Protocol>::_async_accept_handle<AcceptHandler>, cb, boost::asio::placeholders::error));
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::accept(BoostSocket<Protocol> &sock, BoostEndpoint<Protocol> &ep, Error &err)
{
  boost::system::error_code berr;

  _handle.accept(sock._handle, ep, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::listen(int max, Error &err)
{
  boost::system::error_code berr;

  _handle.listen(max, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::bind(BoostEndpoint<Protocol> const &ep, Error &err)
{
  boost::system::error_code berr;

  _handle.bind(ep, berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::close(Error &err)
{
  boost::system::error_code berr;

  _handle.close(berr);
  boost2moost_error(err, berr);
}

template<typename Protocol>
void BoostSocketAcceptor<Protocol>::cancel(Error &err)
{
  boost::system::error_code berr;

  _handle.cancel(berr);
  boost2moost_error(err, berr);
}


} // stash
} // moost

#endif /* !BOOSTSOCKETACCEPTOR_HPP */
