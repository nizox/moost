#ifndef BASICSOCKETACCEPTOR_HPP
#define BASICSOCKETACCEPTOR_HPP

#include "moost/stash/BasicDispatcher.hpp"
#include "moost/utils/Error.hpp"

namespace moost {
namespace stash {

template<typename Protocol, typename Implementation, typename DispatcherImplementation>
class BasicSocketAcceptor
{
 public:
  typedef Protocol ProtocolType;
  typedef typename Protocol::Socket SocketType;
  typedef typename Protocol::Endpoint EndpointType;
  typedef typename Implementation::NativeType NativeType;

  typedef typename Implementation::ReuseAddress ReuseAddress;
  typedef typename Implementation::NonBlocking NonBlocking;

  BasicSocketAcceptor(BasicDispatcher<DispatcherImplementation> &dispatcher)
      : _implem(dispatcher._implem) {}

  BasicSocketAcceptor(BasicDispatcher<DispatcherImplementation> &dispatcher,
                      ProtocolType const &protocol)
      : _implem(dispatcher._implem, protocol._implem) {}

  BasicSocketAcceptor(BasicDispatcher<DispatcherImplementation> &dispatcher,
                      EndpointType const &ep, ReuseAddress reuse = ReuseAddress(true))
      : _implem(dispatcher._implem, ep._implem, reuse) {}

  ~BasicSocketAcceptor() {}

  template<typename Option>
  inline void set_option(Option opt, Error &err)
  {
    _implem.set_option(opt, err);
  }

  inline EndpointType local_endpoint(Error &err)
  {
    return _implem.local_endpoint(err);
  }

  inline EndpointType remote_endpoint(Error &err)
  {
    return _implem.remote_endpoint(err);
  }

  template<typename Option>
  inline Option get_option(Option opt, Error &err)
  {
    return _implem.set_option(opt, err);
  }

  inline void open(ProtocolType const &protocol, Error &err)
  {
    _implem.open(protocol._implem, err);
  }

  inline void accept(typename ProtocolType::Socket &sock, EndpointType &ep, Error &err)
  {
    _implem.accept(sock._implem, ep._implem, err);
  }

  template<typename AcceptHandler>
  inline void async_accept(SocketType &sock, EndpointType &ep, AcceptHandler cb)
  {
    _implem.async_accept(sock._implem, ep._implem, cb);
  }

  inline void bind(EndpointType &ep, Error &err)
  {
    _implem.bind(ep._implem, err);
  }

  inline void listen(int max, Error &err)
  {
    _implem.listen(max, err);
  }

  inline void close(Error &err)
  {
    _implem.close(err);
  }

  inline void cancel(Error &err)
  {
    _implem.cancel(err);
  }

  inline NativeType native() const
  {
    return _implem.native();
  }

  inline void assign(NativeType fd, Error &err)
  {
    _implem.assign(fd, err);
  }

 private:
  Implementation _implem;
};

} // stash
} // moost

#endif /* !BASICSOCKETACCEPTOR_HPP */
