#ifndef LOCAL_HPP
#define LOCAL_HPP

#include "moost/stash/BasicProtocol.hpp"
#include "moost/stash/BasicEndpoint.hpp"
#include "moost/stash/BasicSocketDatagram.hpp"
#include "moost/stash/BasicSocketStream.hpp"
#include "moost/stash/BasicSocketAcceptor.hpp"

namespace moost {
namespace stash {
namespace local {

struct datagram: public BasicProtocol<datagram, Traits::ImplemLocal<datagram>::TYPE> {
  typedef BasicEndpoint<datagram, Traits::ImplemEndpoint<datagram>::TYPE> Endpoint;
  typedef BasicSocketDatagram<datagram, Traits::ImplemSocketDatagram<datagram>::TYPE, Traits::ImplemDispatcher> Socket;

  inline datagram() : BasicProtocol<datagram, Traits::ImplemLocal<datagram>::TYPE>() {}
  inline datagram(Traits::ImplemLocal<datagram>::TYPE const &protocol)
      : BasicProtocol<datagram, Traits::ImplemLocal<datagram>::TYPE>(protocol) {}
};

struct stream: public BasicProtocol<stream, Traits::ImplemLocal<stream>::TYPE> {
  typedef BasicEndpoint<stream, Traits::ImplemEndpoint<stream>::TYPE> Endpoint;
  typedef BasicSocketStream<stream, Traits::ImplemSocketStream<stream>::TYPE, Traits::ImplemDispatcher> Socket;
  typedef BasicSocketAcceptor<stream, Traits::ImplemSocketAcceptor<stream>::TYPE, Traits::ImplemDispatcher> Acceptor;

  inline stream() : BasicProtocol<stream, Traits::ImplemLocal<stream>::TYPE>() {}
  inline stream(Traits::ImplemLocal<stream>::TYPE const &protocol)
      : BasicProtocol<stream, Traits::ImplemLocal<stream>::TYPE>(protocol) {}
};

template<typename Protocol>
inline void SocketPair(typename Protocol::Socket &s1,
                       typename Protocol::Socket &s2,
                       Error &err)
{
  typename Traits::ImplemSocketPair<Protocol>::TYPE i;

  i(s1._implem, s2._implem, err);
}

} // Local
} // stash
} // moost

#endif /* !Local_HPP */
