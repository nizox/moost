#ifndef UNIXSOCKETDATAGRAM_H
#define UNIXSOCKETDATAGRAM_H

#include "moost/stash/Buffer.hpp"
#include "moost/stash/unix/UnixSocket.hpp"
#include "moost/stash/unix/UnixDispatcher.hpp"

namespace moost {
namespace stash {

template<typename Protocol, typename Dispatcher>
class UnixSocketDatagram: public UnixSocket<Protocol, Dispatcher>
{
 public:
  UnixSocketDatagram(Dispatcher &);
  UnixSocketDatagram(Dispatcher &, UnixProtocol<Protocol> const &);
  UnixSocketDatagram(Dispatcher &, UnixEndpoint<Protocol> const &);
  ~UnixSocketDatagram();

  template<typename ReceiveHandler>
  void async_receive_from(Buffer &, UnixEndpoint<Protocol> &, ReceiveHandler);
  template<typename SendHandler>
  void async_send_to(Buffer const &, UnixEndpoint<Protocol> const &, SendHandler);
  size_t receive_from(Buffer &, UnixEndpoint<Protocol> &, Error &);
  size_t send_to(Buffer const &, UnixEndpoint<Protocol> const &, Error &);
};

template<typename Protocol, typename Dispatcher, typename ReceiveHandler>
class ReceiveFromOperation: public IOperation
{
 public:
  inline ReceiveFromOperation(UnixSocketDatagram<Protocol, Dispatcher> &socket,
                       Buffer &buf,
                       UnixEndpoint<Protocol> &endpoint,
                       ReceiveHandler cb)
      : _socket(socket), _buf(buf), _endpoint(endpoint), _cb(cb) {}
  inline virtual ~ReceiveFromOperation() {}

  inline virtual int operator()()
  {
    size_t bytes;
    Error err;
    
    bytes = _socket.receive_from(_buf, _endpoint, err);
    if (err == EAGAIN)
      return (0);
    _cb(err);
    return (1);
  }

 private:
  UnixSocketDatagram<Protocol, Dispatcher> &_socket;
  Buffer _buf;
  UnixEndpoint<Protocol> &_endpoint;
  ReceiveHandler _cb;
};

template<typename Protocol, typename Dispatcher, typename SendHandler>
class SendToOperation: public IOperation
{
 public:
  inline SendToOperation(UnixSocketDatagram<Protocol, Dispatcher> &socket,
                  Buffer const &buf,
                  UnixEndpoint<Protocol> const &endpoint,
                  SendHandler cb)
      : _socket(socket), _buf(buf), _endpoint(endpoint), _cb(cb) {}
  inline virtual ~SendToOperation() {}

  inline virtual int operator()()
  {
    size_t bytes;
    Error err;
    
    bytes = _socket.send_to(_buf, _endpoint, err);
    if (err == EAGAIN)
      return (0);
    _cb(err);
    return (1);
  }
 private:
  UnixSocketDatagram<Protocol, Dispatcher> &_socket;
  Buffer _buf;
  UnixEndpoint<Protocol> _endpoint;
  SendHandler _cb;
};

} // stash
} // moost



#endif /* !UNIXSOCKETDATAGRAM_H */
