#ifndef UNIXSOCKETDATAGRAM_HPP
#define UNIXSOCKETDATAGRAM_HPP

#include "moost/stash/unix/UnixSocketDatagram.h"
#include "moost/stash/unix/utils.hpp"

#include <cerrno>

namespace moost {
namespace stash {

template<typename Protocol, typename Dispatcher>
UnixSocketDatagram<Protocol, Dispatcher>::UnixSocketDatagram(Dispatcher &dispatcher)
    : UnixSocket<Protocol, Dispatcher>(dispatcher) 
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketDatagram<Protocol, Dispatcher>::UnixSocketDatagram(Dispatcher &dispatcher,
                                                   UnixProtocol<Protocol> const &protocol)
    : UnixSocket<Protocol, Dispatcher>(dispatcher, protocol)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketDatagram<Protocol, Dispatcher>::UnixSocketDatagram(Dispatcher &dispatcher,
                                                   UnixEndpoint<Protocol> const &ep)
    : UnixSocket<Protocol, Dispatcher>(dispatcher, ep)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketDatagram<Protocol, Dispatcher>::~UnixSocketDatagram()
{
}

template<typename Protocol, typename Dispatcher>
template<typename ReceiveHandler>
void UnixSocketDatagram<Protocol, Dispatcher>::async_receive_from(Buffer &buf, UnixEndpoint<Protocol> &ep, ReceiveHandler cb)
{
  Error err;

  SPIOperation pi(new ReceiveFromOperation<Protocol, Dispatcher, ReceiveHandler>(*this, buf, ep, cb));
  UnixSocket<Protocol, Dispatcher>::_dispatcher.register_event(Dispatcher::IOEventType::EV_IN, UnixSocket<Protocol, Dispatcher>::_fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
template<typename SendHandler>
void UnixSocketDatagram<Protocol, Dispatcher>::async_send_to(Buffer const &buf, UnixEndpoint<Protocol> const &ep, SendHandler cb)
{
  Error err;

  SPIOperation pi(new SendToOperation<Protocol, Dispatcher, SendHandler>(*this, buf, ep, cb));
  UnixSocket<Protocol, Dispatcher>::_dispatcher.register_event(Dispatcher::IOEventType::EV_OUT, UnixSocket<Protocol, Dispatcher>::_fd, pi, err);
}

template<typename Protocol, typename Dispatcher>
size_t UnixSocketDatagram<Protocol, Dispatcher>::receive_from(Buffer &buf, UnixEndpoint<Protocol> &ep, Error &err)
{
  ssize_t bytes;
  socklen_t addrlen = ep.addrlen();

#if defined (WIN32)
  bytes = ::recvfrom(UnixSocket<Protocol, Dispatcher>::_fd, buf.data<char *>(), buf.size(), 0, ep.addr(), &addrlen);
#else
    bytes = ::recvfrom(UnixSocket<Protocol, Dispatcher>::_fd, buf.data<void *>(), buf.size(), 0, ep.addr(), &addrlen);
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
size_t UnixSocketDatagram<Protocol, Dispatcher>::send_to(Buffer const &buf, UnixEndpoint<Protocol> const &ep, Error &err)
{
  ssize_t bytes;

#if defined (WIN32)
   bytes = ::sendto(UnixSocket<Protocol, Dispatcher>::_fd, buf.data<char *>(), buf.size(), 0, ep.addr(), ep.addrlen());
#else
  bytes = ::sendto(UnixSocket<Protocol, Dispatcher>::_fd, buf.data<void *>(), buf.size(), 0, ep.addr(), ep.addrlen());
#endif
  if (bytes == -1)
  {
    err.assign(errno);
    return 0;
  }
  return bytes;
}

} // stash
} // moost

#endif /* !UNIXSOCKETDATAGRAM_HPP */
