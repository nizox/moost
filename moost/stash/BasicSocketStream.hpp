#ifndef BASICSOCKETSTREAM_HPP
#define BASICSOCKETSTREAM_HPP

#include <streambuf>
#include "moost/stash/Buffer.hpp"
#include "moost/stash/BasicSocket.hpp"

namespace moost {
namespace stash {

template<typename Protocol, typename Implementation, typename DispatcherImplementation>
class BasicSocketStream: public BasicSocket<Protocol, Implementation, DispatcherImplementation>
{
 public:
  typedef Protocol ProtocolType;
  typedef typename Protocol::Endpoint EndpointType;

  BasicSocketStream(BasicDispatcher<DispatcherImplementation> &dispatcher)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher) {}

  BasicSocketStream(BasicDispatcher<DispatcherImplementation> &dispatcher,
                    ProtocolType const &protocol)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher, protocol) {}

  BasicSocketStream(BasicDispatcher<DispatcherImplementation> &dispatcher,
                    EndpointType const &ep)
      : BasicSocket<Protocol, Implementation, DispatcherImplementation>(dispatcher, ep) {}

  ~BasicSocketStream() {}

  template<typename ReceiveHandler>
  inline void async_receive(Buffer &buf, ReceiveHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_receive(buf, cb);
  }

  template<typename SendHandler>
  inline void async_send(Buffer const &buf, SendHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_send(buf, cb);
  }

  template<typename SendHandler>
  inline void async_send_safe(Buffer const &buf, SendHandler cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_send_safe(buf, cb);
  }
  
  template <typename ReadUntilCB>
  inline void async_receive_until(moost::streambuf &sbuff, std::string const &stop, ReadUntilCB cb)
  {
    BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.async_receive_until(sbuff, stop, cb);
  }

  inline size_t receive(Buffer &buf, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.receive(buf, err);
  }

  inline size_t receive_until(moost::streambuf &sbuff, std::string const &stop ,Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.receive_until(sbuff, stop, err);
  }

  inline size_t send(Buffer const &buf, Error &err)
  {
    return BasicSocket<Protocol, Implementation, DispatcherImplementation>::_implem.send(buf, err);
  }
};

} // stash
} // moost

#endif /* !BASICSOCKETSTREAM_HPP */
