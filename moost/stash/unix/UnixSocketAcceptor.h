#ifndef UNIXSOCKETACCEPTOR_H
#define UNIXSOCKETACCEPTOR_H

#include "moost/utils/Error.hpp"
#include "moost/stash/unix/UnixProtocol.hpp"
#include "moost/stash/unix/UnixSocketBase.hpp"
#include "moost/stash/unix/UnixSocket.hpp"
#include "moost/stash/unix/UnixDispatcher.hpp"

namespace moost {
namespace stash {


template <typename Protocol, typename Dispatcher>
class UnixSocketAcceptor: public UnixSocketBase
{
 public:
  typedef int NativeType;

  UnixSocketAcceptor(Dispatcher &);
  UnixSocketAcceptor(Dispatcher &,
                     UnixProtocol<Protocol> const &);
  UnixSocketAcceptor(Dispatcher &, UnixEndpoint<Protocol> const &,
                     UnixSocketBase::ReuseAddress);
  ~UnixSocketAcceptor();

  UnixEndpoint<Protocol> local_endpoint(Error &err);
  UnixEndpoint<Protocol> remote_endpoint(Error &err);

  void set_option(UnixSocketBase::ReuseAddress const &, Error &);
  void get_option(UnixSocketBase::ReuseAddress &, Error &);
  void set_option(UnixSocketBase::NonBlocking const &, Error &);
  void get_option(UnixSocketBase::NonBlocking &, Error &);

  void open(UnixProtocol<Protocol> const &, Error &);
  void listen(int, Error &);
  void accept(UnixSocket<Protocol, Dispatcher> &, UnixEndpoint<Protocol> &, Error &);
  template<typename AcceptHandler>
  void async_accept(UnixSocket<Protocol, Dispatcher> &, UnixEndpoint<Protocol> &, AcceptHandler);
  void bind(UnixEndpoint<Protocol> const &, Error &);
  void close(Error &);
  void cancel(Error &);

  void assign(NativeType, Error &);

  NativeType native() const;

 private:
  Dispatcher &_dispatcher;
  NativeType _fd;
};


template<typename Protocol, typename Dispatcher, typename AcceptHandler>
class AcceptOperation: public IOperation
{
 public:
  inline AcceptOperation(UnixSocketAcceptor<Protocol, Dispatcher> &acceptor,
                  UnixSocket<Protocol, Dispatcher> &socket,
                  UnixEndpoint<Protocol> &endpoint,
                  AcceptHandler cb)
      : _acceptor(acceptor), _socket(socket), _endpoint(endpoint), _cb(cb) {}
  inline virtual ~AcceptOperation() {}

  inline virtual int operator()()
  {
    Error err;
    
    _acceptor.accept(_socket, _endpoint, err);
    if (err == EAGAIN)
      return (0);
    _cb(err);
    return (1);
  }

 private:
  UnixSocketAcceptor<Protocol, Dispatcher> &_acceptor;
  UnixSocket<Protocol, Dispatcher> &_socket;
  UnixEndpoint<Protocol> &_endpoint;
  AcceptHandler _cb;
};


} // stash
} // moost

#endif /* !UNIXSOCKETACCEPTOR_H */
