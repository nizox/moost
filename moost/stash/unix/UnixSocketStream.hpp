#ifndef UNIXSOCKETSTREAM_HPP
#define UNIXSOCKETSTREAM_HPP

#include "moost/stash/unix/UnixSocketStream.h"

#include <algorithm>
#include <cstring>
#include <cerrno>

namespace moost {
namespace stash {

template<typename Protocol, typename Dispatcher>
UnixSocketStream<Protocol, Dispatcher>::UnixSocketStream(Dispatcher &dispatcher)
    :  UnixSocket<Protocol, Dispatcher>(dispatcher)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketStream<Protocol, Dispatcher>::UnixSocketStream(Dispatcher &dispatcher,
                                               UnixProtocol<Protocol> const &protocol)
    :  UnixSocket<Protocol, Dispatcher>(dispatcher, protocol)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketStream<Protocol, Dispatcher>::UnixSocketStream(Dispatcher &dispatcher,
                                               UnixEndpoint<Protocol> const &ep)
    :  UnixSocket<Protocol, Dispatcher>(dispatcher, ep)
{
}

template<typename Protocol, typename Dispatcher>
UnixSocketStream<Protocol, Dispatcher>::~UnixSocketStream()
{
}

template <typename Protocol, typename Dispatcher>
size_t  UnixSocketStream<Protocol, Dispatcher>::receive_until(moost::streambuf &buff, std::string const &stop, Error &err)
{
  ssize_t bytes = 0;
  ssize_t total_bytes = buff.size();
  char *find;
  char bf[512];
  Buffer b(bf, 512);

  while (!err)
  {
    bytes = UnixSocket<Protocol, Dispatcher>::receive(b, err);
    total_bytes += bytes;
    buff.sputn(b.data<char *>(), bytes);
    if ((find = std::search(bf, bf + bytes, stop.begin(), stop.end())) != (bf + bytes))
      break; 
  }
  return total_bytes;
}

template <typename Protocol, typename Dispatcher>
template <typename ReceiveHandlerUntil>
void  UnixSocketStream<Protocol, Dispatcher>::async_receive_until(moost::streambuf &sb, std::string const &stop, ReceiveHandlerUntil cb)
{
  Error err;

  SPIOperation pi(new ReceiveUntilOperation<Protocol, Dispatcher, ReceiveHandlerUntil>(*this, stop, cb));
  UnixSocket<Protocol, Dispatcher>::_dispatcher.register_event(Dispatcher::IOEventType::EV_IN, UnixSocket<Protocol, Dispatcher>::_fd, pi, err);
}


} // stash
} // moost

#endif /* !UNIXSOCKETSTREAM_HPP */
