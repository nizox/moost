#ifndef BASICSOCKET_HPP
#define BASICSOCKET_HPP

#include "moost/stash/BasicDispatcher.hpp"
#include "moost/utils/Error.hpp"

namespace moost {
namespace stash {

namespace local {
  template <typename Protocol>
  void SocketPair(typename Protocol::Socket &, typename Protocol::Socket &, Error &);
}

template<typename Protocol, typename Implementation, typename DispatcherImplementation>
class BasicSocket
{
 public:
  typedef Protocol ProtocolType;
  typedef typename Protocol::Endpoint EndpointType;
  typedef typename Implementation::NativeType NativeType;

  typedef typename Implementation::ReuseAddress ReuseAddress;
  typedef typename Implementation::NonBlocking NonBlocking;

  BasicSocket(BasicDispatcher<DispatcherImplementation> &dispatcher)
      : _implem(dispatcher._implem) {}

  BasicSocket(BasicDispatcher<DispatcherImplementation> &dispatcher,
              ProtocolType const &protocol)
      : _implem(dispatcher._implem, protocol._implem) {}

  BasicSocket(BasicDispatcher<DispatcherImplementation> &dispatcher,
              EndpointType const &ep)
      : _implem(dispatcher._implem, ep._implem) {}

  ~BasicSocket() {}

  template<typename Option>
  inline void set_option(Option const &opt, Error &err)
  {
    _implem.set_option(opt, err);
  }

  template<typename Option>
  inline void get_option(Option &opt, Error &err)
  {
    return _implem.set_option(opt, err);
  }

  inline void open(ProtocolType const &protocol, Error &err)
  {
    _implem.open(protocol._implem, err);
  }

  template<typename ConnectHandler>
  inline void async_connect(EndpointType const &ep, ConnectHandler cb)
  {
    _implem.async_connect(ep._implem, cb);
  }

  inline EndpointType local_endpoint(Error &err)
  {
    return _implem.local_endpoint(err);
  }

  inline EndpointType remote_endpoint(Error &err)
  {
    return _implem.remote_endpoint(err);
  }

  inline void bind(EndpointType const &ep, Error &err)
  {
    _implem.bind(ep._implem, err);
  }

  inline void close(Error &err)
  {
    _implem.close(err);
  }

  inline void connect(EndpointType const &ep, Error &err)
  {
    _implem.connect(ep._implem, err);
  }

  inline void cancel(Error &err)
  {
    _implem.cancel(err);
  }

  inline NativeType native() const
  {
    return _implem.native();
  }

  inline void assign(NativeType val, Error &err)
  {
    _implem.assign(val, err);
  }

 protected:
  Implementation _implem;

  template <typename P, typename I, typename DI> friend class BasicSocketAcceptor;
  template <typename P> friend void local::SocketPair(typename P::Socket &, typename P::Socket &, Error &);
};

} // stash
} // moost

#endif /* !BASICSOCKET_HPP */
