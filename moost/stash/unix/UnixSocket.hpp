#ifndef UNIXSOCKET_HPP
#define UNIXSOCKET_HPP

#ifdef WIN32
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/socket.h>
#include <fcntl.h>
#endif


#include <cerrno>

#include "moost/stash/unix/UnixSocket.h"

namespace moost {
namespace stash {


// TODO throw exception if not working
// always check if the socket is open
template<typename Protocol, typename Dispatcher>
UnixSocket<Protocol, Dispatcher>::UnixSocket(Dispatcher &dispatcher)
    :  _fd(-1), _dispatcher(dispatcher)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocket<Protocol, Dispatcher>::UnixSocket(Dispatcher &dispatcher,
                                   UnixProtocol<Protocol> const &protocol)
    :  _fd(-1), _dispatcher(dispatcher)
{
  Error err;

  open(protocol, err);
}

template<typename Protocol, typename Dispatcher>
UnixSocket<Protocol, Dispatcher>::UnixSocket(Dispatcher &dispatcher,
                                   UnixEndpoint<Protocol> const &ep)
    : _fd(-1), _dispatcher(dispatcher)
{
  Error err;

  open(ep.protocol(), err);
  bind(ep, err);
}

template<typename Protocol, typename Dispatcher>
UnixSocket<Protocol, Dispatcher>::~UnixSocket()
{
  if (_fd != -1)
  {
    Error err;
    close(err);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::get_option(UnixSocketBase::NonBlocking &opt, Error &err)
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
void UnixSocket<Protocol, Dispatcher>::set_option(UnixSocketBase::NonBlocking const &opt, Error &err)
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
void UnixSocket<Protocol, Dispatcher>::get_option(UnixSocketBase::ReuseAddress &opt, Error &err)
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
void UnixSocket<Protocol, Dispatcher>::set_option(UnixSocketBase::ReuseAddress const &opt, Error &err)
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
UnixEndpoint<Protocol> UnixSocket<Protocol, Dispatcher>::local_endpoint(Error &err)
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
UnixEndpoint<Protocol> UnixSocket<Protocol, Dispatcher>::remote_endpoint(Error &err)
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
template<typename ReceiveHandler>
void UnixSocket<Protocol, Dispatcher>::async_receive(Buffer &buf, ReceiveHandler cb)
{
  Error err;

  SPIOperation pi(new ReceiveOperation<Protocol, Dispatcher, ReceiveHandler>(*this, buf, cb));
  _dispatcher.register_event(Dispatcher::IOEventType::EV_IN, _fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
template<typename SendHandler>
void UnixSocket<Protocol, Dispatcher>::async_send(Buffer const &buf, SendHandler cb)
{
    Error err;

    SPIOperation pi(new SendOperation<Protocol, Dispatcher, SendHandler>(*this, buf, cb));
    _dispatcher.register_event(Dispatcher::IOEventType::EV_OUT, _fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
template<typename SendHandler>
void UnixSocket<Protocol, Dispatcher>::async_send_safe(Buffer const &buf, SendHandler cb)
{
    Error err;

    SPIOperation pi(new SendSafeOperation<Protocol, Dispatcher, SendHandler>(*this, buf, cb));
    _dispatcher.register_event(Dispatcher::IOEventType::EV_OUT, _fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
size_t UnixSocket<Protocol, Dispatcher>::receive(Buffer &buf, Error &err)
{
  ssize_t bytes;

#if defined(WIN32)
  bytes = ::recv(_fd, buf.data<char *>(), buf.size(), 0);
#else
  bytes = ::recv(_fd, buf.data<void *>(), buf.size(), 0);
#endif
  if (bytes == -1)
  {
    err.assign(errno);
    return 0;
  }
  if (bytes == 0)
    err.assign(ESHUTDOWN);
  return bytes;
}

template<typename Protocol, typename Dispatcher>
size_t UnixSocket<Protocol, Dispatcher>::send(Buffer const &buf, Error &err)
{
  ssize_t bytes;

#if defined(WIN32)
  bytes = ::send(_fd, buf.data<char *>(), buf.size(), MSG_NOSIGNAL);
#else
  bytes = ::send(_fd, buf.data<void *>(), buf.size(), MSG_NOSIGNAL);
#endif
  if (bytes == -1)
  {
    err.assign(errno);
    return 0;
  }
  if (bytes == 0)
    err.assign(ESHUTDOWN);
  return bytes;
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::open(UnixProtocol<Protocol> const &protocol, Error &err)
{
  if (!err)
  {
    _fd = ::socket(protocol.domain(), protocol.type(), protocol.protocol());
    if (_fd == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
template<typename ConnectHandler>
void UnixSocket<Protocol, Dispatcher>::async_connect(UnixEndpoint<Protocol> const &ep, ConnectHandler cb)
{
  Error err;

  SPIOperation pi(new ConnectOperation<Protocol, Dispatcher, ConnectHandler>(*this, ep, cb));
  _dispatcher.register_event(Dispatcher::IOEventType::EV_OUT, _fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::bind(UnixEndpoint<Protocol> const &ep, Error &err)
{
  if (!err)
  {
    int ret;

    ret = ::bind(_fd, ep.addr(), ep.addrlen());
    if (ret == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::close(Error &err)
{
  //std::cout << "closing" << std::endl;
  cancel(err);
  if (!err)
  {
    int ret;

    ret = ::close(_fd);
    if (ret == -1)
      err.assign(errno);
    _fd = -1;
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::connect(UnixEndpoint<Protocol> const &ep, Error &err)
{
  if (!err)
  {
    int ret;

    ret = ::connect(_fd, ep.addr(), ep.addrlen());
    if (ret == -1)
      err.assign(errno);
  }
}

template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::cancel(Error &err)
{
  if (!err)
  {
    _dispatcher.cancel_event(Dispatcher::IOEventType::EV_IN, _fd, err);
    _dispatcher.cancel_event(Dispatcher::IOEventType::EV_OUT, _fd, err);
  }
}


template<typename Protocol, typename Dispatcher>
void UnixSocket<Protocol, Dispatcher>::assign(int fd, Error &err)
{
 // if (_fd != -1)
 //   close(err);
  if (!err)
    _fd = fd;
}

template<typename Protocol, typename Dispatcher>
typename UnixSocket<Protocol, Dispatcher>::NativeType UnixSocket<Protocol, Dispatcher>::native() const
{
  return _fd;
}

} // stash
} // moost

#endif /* !UNIXSOCKET_HPP */
