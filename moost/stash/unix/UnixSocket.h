#ifndef UNIXSOCKET_H
#define UNIXSOCKET_H

#include "moost/utils/Error.hpp"
#include "moost/stash/unix/UnixProtocol.hpp"
#include "moost/stash/unix/UnixSocketBase.hpp"
#include "moost/stash/unix/UnixEndpoint.hpp"
#include "moost/stash/unix/UnixDispatcher.hpp"

namespace moost {
namespace stash {


template<typename Protocol, typename Dispatcher>
class UnixSocket: public UnixSocketBase
{
 public:
  typedef int NativeType;

  UnixSocket(Dispatcher &);
  UnixSocket(Dispatcher &, UnixProtocol<Protocol> const &);
  UnixSocket(Dispatcher &, UnixEndpoint<Protocol> const &);
  ~UnixSocket();

  void set_option(UnixSocketBase::ReuseAddress const &, Error &);
  void get_option(UnixSocketBase::ReuseAddress &, Error &);
  void set_option(UnixSocketBase::NonBlocking const &, Error &);
  void get_option(UnixSocketBase::NonBlocking &, Error &);

  UnixEndpoint<Protocol> local_endpoint(Error &err);
  UnixEndpoint<Protocol> remote_endpoint(Error &err);

  template<typename ReceiveHandler>
  void async_receive(Buffer &, ReceiveHandler);
  template<typename SendHandler>
  void async_send(Buffer const &, SendHandler);

  template<typename SendHandler>
  void async_send_safe(Buffer const &, SendHandler);

  size_t receive(Buffer &, Error &);
  size_t send(Buffer const &, Error &);

  void open(UnixProtocol<Protocol> const &, Error &);
  template<typename ConnectHandler>
  void async_connect(UnixEndpoint<Protocol> const &, ConnectHandler);
  void bind(UnixEndpoint<Protocol> const &, Error &);
  void close(Error &);
  void connect(UnixEndpoint<Protocol> const &, Error &);
  void cancel(Error &);

  void assign(int, Error &);

  NativeType native() const;

 protected:
  NativeType _fd;
  Dispatcher &_dispatcher;
};

template<typename Protocol, typename Dispatcher, typename ConnectHandler>
class ConnectOperation: public IOperation
{
 public:
  inline ConnectOperation(UnixSocket<Protocol, Dispatcher> &socket,
                   UnixEndpoint<Protocol> const &endpoint,
                   ConnectHandler cb)
      : _socket(socket),  _endpoint(endpoint), _cb(cb) {}
  inline virtual ~ConnectOperation() {}

  inline virtual int operator()()
  {
    Error err;
    
    _socket.connect(_endpoint, err);
    _cb(err);
    return (1);
  }

 private:
  UnixSocket<Protocol, Dispatcher> &_socket;
  UnixEndpoint<Protocol> _endpoint;
  ConnectHandler _cb;
};

template<typename Protocol, typename Dispatcher, typename ReceiveHandler>
class ReceiveOperation: public IOperation
{
 public:
  inline ReceiveOperation(UnixSocket<Protocol, Dispatcher> &socket,
                   Buffer &buf,
                   ReceiveHandler cb)
      : _socket(socket),  _buf(buf), _cb(cb) {}
  inline virtual ~ReceiveOperation() {}

  inline virtual int operator()()
  {
    size_t bytes;
    Error err;
    
    bytes = _socket.receive(_buf, err);
    if (err.code() == EAGAIN)
      return (0);
    _cb(bytes, err);
    return (1);
  }

 private:
  UnixSocket<Protocol, Dispatcher> &_socket;
  Buffer _buf;
  ReceiveHandler _cb;
};

template<typename Protocol, typename Dispatcher, typename SendHandler>
class SendOperation: public IOperation
{
 public:
  inline SendOperation(UnixSocket<Protocol, Dispatcher> &socket,
                Buffer const &buf,
                SendHandler cb)
      : _socket(socket), _buf(buf), _cb(cb) {}
  inline virtual ~SendOperation() {}

  inline virtual int operator()()
  {
    size_t bytes;
    Error err;

    //std::cout << "send operation " << _buf.size() << std::endl;
    bytes = _socket.send(_buf, err);
    if (err.code() == EAGAIN)
      return (0);
    _cb(bytes, err);
    return (1);
  }

 private:
  UnixSocket<Protocol, Dispatcher> &_socket;
  Buffer _buf;
  SendHandler _cb;
};

template<typename Protocol, typename Dispatcher, typename SendHandler>
class SendSafeOperation: public IOperation
{
 public:
  inline SendSafeOperation(UnixSocket<Protocol, Dispatcher> &socket,
                Buffer const &buf,
                SendHandler cb)
      : _socket(socket), _buf(buf), _cb(cb), _sent(0) {}
  inline virtual ~SendSafeOperation() {}

  inline virtual int operator()()
  {
    size_t bytes;
    Error err;

    if (_sent)
    {
      Buffer buf(_buf.data<char *>() + _sent, _buf.size() - _sent);

      bytes = _socket.send(buf, err);
    }
    else
      bytes = _socket.send(_buf, err);
    if (err.code() == EAGAIN)
      return (0);
    _sent += bytes;
    if (_sent != _buf.size())
      return (0);
    _cb(_sent, err);
    return (1);
  }

 private:
  UnixSocket<Protocol, Dispatcher> &_socket;
  Buffer _buf;
  SendHandler _cb;
  size_t _sent;
};


} // stash
} // moost

#endif /* !UNIXSOCKET_H */
