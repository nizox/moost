#ifndef UNIXSOCKETACCEPTOR_HPP
#define UNIXSOCKETACCEPTOR_HPP

#include "moost/stash/unix/UnixSocketAcceptor.h"

#include <fcntl.h>

namespace moost {
namespace stash {

template<typename Protocol, typename Dispatcher>
UnixSocketAcceptor<Protocol, Dispatcher>::UnixSocketAcceptor(Dispatcher &dispatcher)
    : _dispatcher(dispatcher), _fd(-1)
{
}

template<typename Protocol, typename Dispatcher>
UnixEndpoint<Protocol> UnixSocketAcceptor<Protocol, Dispatcher>::local_endpoint(Error &err)
{
  UnixEndpoint<Protocol> ep;

  if (!err)
  {
    int ret;
    socklen_t len = ep.addrlen();

    ret = getsockname(_fd, ep.addr(), &len);

    if (ret == -1)
      err.assign(errno);
  }
  return ep;
}

template<typename Protocol, typename Dispatcher>
UnixEndpoint<Protocol> UnixSocketAcceptor<Protocol, Dispatcher>::remote_endpoint(Error &err)
{
  UnixEndpoint<Protocol> ep;

  if (!err)
  {
    int ret;
    socklen_t len = ep.addrlen();

    ret = getpeername(_fd, ep.addr(), &len);

    if (ret == -1)
      err.assign(errno);
  }
  return ep;
}

template<typename Protocol, typename Dispatcher>
UnixSocketAcceptor<Protocol, Dispatcher>::UnixSocketAcceptor(Dispatcher &dispatcher,
                                                   UnixProtocol<Protocol> const &protocol)
    : _dispatcher(dispatcher), _fd(-1)
{
  Error err;

  open(protocol, err);
}

template<typename Protocol, typename Dispatcher>
UnixSocketAcceptor<Protocol, Dispatcher>::UnixSocketAcceptor(Dispatcher &dispatcher,
                                                 UnixEndpoint<Protocol> const &ep,
                                                 UnixSocketBase::ReuseAddress reuse)
    : _dispatcher(dispatcher), _fd(-1)
{
  Error err;

  open(ep.protocol(), err);
  set_option(reuse, err);
  bind(ep, err);
  listen(SOMAXCONN, err);
}

template<typename Protocol, typename Dispatcher>
UnixSocketAcceptor<Protocol, Dispatcher>::~UnixSocketAcceptor()
{
  if (_fd != -1)
  {
    Error err;

    close(err);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::get_option(UnixSocketBase::NonBlocking &opt, Error &err)
{
#if !defined(WIN32)
  if (!err)
  {
    int ret;
    int b;

    ret = fcntl(_fd, F_GETFL, 0);
    if (ret == -1)
      err.assign(errno);
    else
      opt = (ret & O_NONBLOCK) ? true : false;
  }
#endif
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::set_option(UnixSocketBase::NonBlocking const &opt, Error &err)
{
  if (!err)
  {
    int ret;	
#if defined(WIN32)
    unsigned long mode = (opt == true) ? 1 : 0;

    ret = ioctlsocket(_fd, FIONBIO, &mode);
#else
    ret = fcntl(_fd, F_GETFL, 0);
#endif
    if (ret == -1)
    {
      err.assign(errno);
      return ;
    }
#if !defined(WIN32)
    ret = fcntl(_fd, F_SETFL, (opt == true) ? ret | O_NONBLOCK : ret & ~O_NONBLOCK);
    if (ret == -1)
      err.assign(errno);
#endif
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::get_option(UnixSocketBase::ReuseAddress &opt, Error &err)
{
  if (!err)
  {
    int ret;
    int b;

#if defined(WIN32)
    ret = getsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&b, sizeof(b));
#else
    ret = getsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &b, sizeof(b));
#endif
    if (ret == -1)
      err.assign(errno);
    opt = b;
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::set_option(UnixSocketBase::ReuseAddress const &opt, Error &err)
{
  if (!err)
  {
    int ret;
    int b = opt;

    ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&b, sizeof(b));
    if (ret == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::open(UnixProtocol<Protocol> const &protocol, Error &err)
{
  if (!err)
  {
    _fd = ::socket(protocol.domain(), protocol.type(), protocol.protocol());
    if (_fd == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
template<typename AcceptHandler>
void UnixSocketAcceptor<Protocol, Dispatcher>::async_accept(UnixSocket<Protocol, Dispatcher> &sock, UnixEndpoint<Protocol> &ep, AcceptHandler cb)
{
  Error err;

  SPIOperation pi(new AcceptOperation<Protocol, Dispatcher, AcceptHandler>(*this, sock, ep, cb));
  _dispatcher.register_event(Dispatcher::IOEventType::EV_IN, _fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::accept(UnixSocket<Protocol, Dispatcher> &sock, UnixEndpoint<Protocol> &ep, Error &err)
{
  if (!err)
  {
    int fd;
    socklen_t len = ep.addrlen();

    fd = ::accept(_fd, ep.addr(), &len);
    //std::cout << "accepting " << _fd << " " << fd << " " << errno << std::endl;
    if (fd != -1)
      sock.assign(fd, err);
    else
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::listen(int max, Error &err)
{
  if (!err)
  {
    int ret = ::listen(_fd, max);

    if (ret == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::bind(UnixEndpoint<Protocol> const &ep, Error &err)
{
  if (!err)
  {
    int ret = ::bind(_fd, ep.addr(), ep.addrlen());

    if (ret == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::close(Error &err)
{
  if (!err)
  {
    int ret;

    cancel(err);
    ret = ::close(_fd);
    if (ret)
      err.assign(errno);
    _fd = -1;
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::cancel(Error &err)
{
  if (!err)
  {
    _dispatcher.cancel_event(Dispatcher::IOEventType::EV_IN, _fd, err);
    _dispatcher.cancel_event(Dispatcher::IOEventType::EV_OUT, _fd, err);
  }
}

template<typename Protocol, typename Dispatcher>
typename UnixSocketAcceptor<Protocol, Dispatcher>::NativeType UnixSocketAcceptor<Protocol, Dispatcher>::native() const
{
  return _fd;
}

template<typename Protocol, typename Dispatcher>
void UnixSocketAcceptor<Protocol, Dispatcher>::assign(typename UnixSocketAcceptor<Protocol, Dispatcher>::NativeType val, Error &err)
{
 // if (_fd != -1)
 //   close(err);
  if (!err)
    _fd = val;
}


} // stash
} // moost

#endif /* !UNIXSOCKETACCEPTOR_HPP */
