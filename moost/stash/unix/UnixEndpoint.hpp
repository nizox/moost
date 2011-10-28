#ifndef UNIXENDPOINT_HPP
#define UNIXENDPOINT_HPP

#include "UnixProtocol.hpp"

#include <string>

#if defined(WIN32)
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace moost {
namespace stash {

template<typename Protocol>
class UnixEndpoint
{

 public:
  UnixEndpoint() : _addr()
  {
    _addr.v4.sin_family = AF_INET;
    _addr.v4.sin_port = 0;
    _addr.v4.sin_addr.s_addr = INADDR_ANY;
  }

  UnixEndpoint(UnixEndpoint<Protocol> const &endpoint)
      : _addr(endpoint._addr)
  {
  }

  UnixEndpoint(std::string const &address, unsigned short port)
  {
    if (::inet_aton(address.c_str(), &_addr.v4.sin_addr) != 0)
    {
      _addr.v4.sin_family = AF_INET;
      _addr.v4.sin_port = htons(port);
    }
  }

  UnixEndpoint(UnixProtocol<Protocol> const &protocol, unsigned short port)
  {
    if (protocol.domain() == PF_INET6)
    {
      in6_addr tmp = IN6ADDR_ANY_INIT;

      _addr.v6.sin6_family = AF_INET6;
      _addr.v6.sin6_port = htons(port);
      _addr.v6.sin6_flowinfo = 0;
      _addr.v6.sin6_addr = tmp;
      _addr.v6.sin6_scope_id = 0;
    }
    else
    {
      _addr.v4.sin_family = AF_INET;
      _addr.v4.sin_port = htons(port);
      _addr.v4.sin_addr.s_addr = INADDR_ANY;
    }
  }

  inline UnixProtocol<Protocol> protocol() const
  {
    if (_addr.base.sa_family == AF_INET6)
      return (UnixIP<Protocol>::v6());
    return (UnixIP<Protocol>::v4());
  }

  void address(std::string const &address)
  {
    if (::inet_aton(address.c_str(), &_addr.v4.sin_addr) != 0)
      _addr.v4.sin_family = AF_INET;
  }

  std::string address() const
  {
    if (_addr.base.sa_family == AF_INET)
      return inet_ntoa(_addr.v4.sin_addr);
    return "ipv6-address";
  }

  void port(unsigned short p)
  {
    if (_addr.base.sa_family == AF_INET)
      _addr.v4.sin_port = htons(p);
    else
      _addr.v6.sin6_port = htons(p);
  }

  unsigned short port() const
  {
    if (_addr.base.sa_family == AF_INET)
      return ntohs(_addr.v4.sin_port);
    return ntohs(_addr.v6.sin6_port);
  }

  sockaddr *addr()
  {
    return reinterpret_cast<sockaddr *>(&_addr);
  }

  sockaddr const *addr() const
  {
    return reinterpret_cast<sockaddr const *>(&_addr);
  }

  socklen_t addrlen()
  {
    if (_addr.base.sa_family == AF_INET6)
      return sizeof(_addr.v6);
    return sizeof(_addr.v4);
  }

  socklen_t addrlen() const
  {
    if (_addr.base.sa_family == AF_INET6)
      return sizeof(_addr.v6);
    return sizeof(_addr.v4);
  }


 private:
  union type {
    struct    sockaddr base;
    struct    sockaddr_in v4;
    struct    sockaddr_in6 v6;
  } _addr;
};

} // stash
} // moost

#endif /* !UNIXENDPOINT_HPP */
