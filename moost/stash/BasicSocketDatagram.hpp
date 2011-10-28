#ifndef BASICSOCKETDATAGRAM_HPP
#define BASICSOCKETDATAGRAM_HPP

#include "moost/stash/Buffer.hpp"
#include "moost/stash/BasicSocket.hpp"

namespace moost {
namespace stash {

template<typename Protocol, typename Implementation, typename DispatcherImplementation>
class BasicSocketDatagram: public BasicSocket<Protocol, Implementation, DispatcherImplementation>
{
 public:
  typedef Protocol ProtocolType;
  typedef typename Protocol::Endpoint EndpointType;

  BasicSocketDatagram(BasicDispatcher<DispatcherImplementation> &dispatcher)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher) {}

  BasicSocketDatagram(BasicDispatcher<DispatcherImplementation> &dispatcher,
                      ProtocolType const &protocol)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher, protocol) {}

  BasicSocketDatagram(BasicDispatcher<DispatcherImplementation> &dispatcher,
                      EndpointType const &ep)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher, ep) {}

  ~BasicSocketDatagram() {}

  template<typename ReceiveHandler>
  inline void async_receive(Buffer &buf, ReceiveHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_receive(buf, cb);
  }

  template<typename ReceiveHandler>
  inline void async_receive_from(Buffer &buf, EndpointType &ep, ReceiveHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_receive_from(buf, ep._implem, cb);
  }

  template<typename SendHandler>
  inline void async_send(Buffer const &buf, SendHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_send(buf, cb);
  }

  template<typename SendHandler>
  inline void async_send_to(Buffer const &buf, EndpointType const &ep, SendHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_send_to(buf, ep._implem, cb);
  }

  inline size_t receive(Buffer &buf, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.receive(buf, err);
  }

  inline size_t receive_from(Buffer &buf, EndpointType &ep, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.receive_from(buf, ep._implem, err);
  }

  inline size_t send(Buffer const &buf, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.send(buf, err);
  }

  inline size_t send_to(Buffer const &buf, EndpointType const &ep, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.send_to(buf, ep._implem, err);
  }
};

} // stash
} // moost

#endif /* !BASICSOCKETDATAGRAM_HPP */
