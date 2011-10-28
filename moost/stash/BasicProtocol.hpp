#ifndef BASICPROTOCOL_HPP
#define BASICPROTOCOL_HPP

namespace moost {
namespace stash {

template <typename Protocol, typename Implementation>
class BasicProtocol {
 public:
  typedef Protocol ProtocolType;

  inline int domain()
  {
    return _implem.domain(); // AF_INET.
  }

  inline int type()
  {
    return _implem.type(); // SOCK_DGRAM/STREAM
  }

  inline int protocol()
  {
    return _implem.protocol(); // tcp/udp
  }

 protected:
  inline BasicProtocol() : _implem() {}
  inline BasicProtocol(Implementation const &implem) : _implem(implem) {}
  inline ~BasicProtocol() {}

  Implementation _implem;

  template <typename P, typename I> friend class BasicQuery;
  template <typename P, typename I> friend class BasicEndpoint;
  template <typename P, typename I, typename DI> friend class BasicSocket;
  template <typename P, typename I, typename DI> friend class BasicSocketAcceptor;
};

} // !stash
} // !moost

#endif // !BASICPROTOCOL_HPP
