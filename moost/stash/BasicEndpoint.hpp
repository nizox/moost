#ifndef BASICENDPOINT_HPP
#define BASICENDPOINT_HPP

#include <iostream>
#include <string>

namespace moost {
namespace stash {

template <typename Protocol, typename Implementation>
class BasicEndpoint
{
 public:
  typedef Protocol ProtocolType;

  BasicEndpoint() : _implem() {}

  BasicEndpoint(ProtocolType const &protocol, unsigned short port)
      : _implem(protocol._implem, port) {}

  BasicEndpoint(std::string const &address, unsigned short port)
      : _implem(address, port) {}

  BasicEndpoint(Implementation const &implem) : _implem(implem) {}

  ~BasicEndpoint() {}

  inline ProtocolType protocol()
  {
    return ProtocolType(_implem.protocol());
  }

  inline std::string address() const
  {
    return _implem.address();
  }

  inline void address(std::string const &a)
  {
    _implem.address(a);
  }

  inline unsigned short port() const
  {
    return _implem.port();
  }

  inline void port(unsigned short p)
  {
    _implem.port(p);
  }

 private:
  Implementation _implem;

  template <typename P, typename I, typename DI> friend class BasicSocket;
  template <typename P, typename I, typename DI> friend class BasicSocketDatagram;
  template <typename P, typename I, typename DI> friend class BasicSocketStream;
  template <typename P, typename I, typename DI> friend class BasicSocketAcceptor;

};

template <typename P, typename I> 
std::ostream &operator<<(std::ostream &out, BasicEndpoint<P, I> const &ep)
{
  out << ep.address() << ":" << ep.port();
  return out;
}

} // stash
} // moost

#endif /* !BASICENDPOINT_HPP */
