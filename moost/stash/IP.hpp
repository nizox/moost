#ifndef IP_HPP
#define IP_HPP

#include "moost/stash/BasicProtocol.hpp"
#include "moost/stash/BasicEndpoint.hpp"
#include "moost/stash/BasicSocketDatagram.hpp"
#include "moost/stash/BasicSocketStream.hpp"
#include "moost/stash/BasicSocketAcceptor.hpp"
#include "moost/stash/BasicResolver.hpp"

namespace moost {
namespace stash {
namespace IP {

template <typename Protocol, typename Implementation>
class BasicIP: public BasicProtocol<Protocol, Implementation>
{
 public:
  typedef Protocol ProtocolType;

  static ProtocolType v4()
  {
    return ProtocolType(Implementation::v4());
  }

  static ProtocolType v6()
  {
    return ProtocolType(Implementation::v6());
  }

 protected:
  inline BasicIP() : BasicProtocol<Protocol, Implementation>() {}
  inline BasicIP(Implementation const &implem) : BasicProtocol<Protocol, Implementation>(implem) {}
  inline ~BasicIP() {}
};

struct UDP: public BasicIP<UDP, Traits::ImplemIP<UDP>::TYPE> {
  typedef BasicEndpoint<UDP, Traits::ImplemEndpoint<UDP>::TYPE> Endpoint;
  typedef BasicSocketDatagram<UDP, Traits::ImplemSocketDatagram<UDP>::TYPE, Traits::ImplemDispatcher> Socket;
  typedef BasicResolver<UDP, Traits::ImplemResolver<UDP>::TYPE, Traits::ImplemDispatcher> Resolver;

  inline UDP(Traits::ImplemIP<UDP>::TYPE const &protocol)
      : BasicIP<UDP, Traits::ImplemIP<UDP>::TYPE>(protocol) {}
};

struct TCP: public BasicIP<TCP, Traits::ImplemIP<TCP>::TYPE> {
  typedef BasicEndpoint<TCP, Traits::ImplemEndpoint<TCP>::TYPE> Endpoint;
  typedef BasicSocketStream<TCP, Traits::ImplemSocketStream<TCP>::TYPE, Traits::ImplemDispatcher> Socket;
  typedef BasicResolver<TCP, Traits::ImplemResolver<TCP>::TYPE, Traits::ImplemDispatcher> Resolver;
  typedef BasicSocketAcceptor<TCP, Traits::ImplemSocketAcceptor<TCP>::TYPE, Traits::ImplemDispatcher> Acceptor;

  inline TCP(Traits::ImplemIP<TCP>::TYPE const &protocol)
      : BasicIP<TCP, Traits::ImplemIP<TCP>::TYPE>(protocol) {}
};

} // IP
} // stash
} // moost

#endif /* !IP_HPP */
