#ifndef UNIXPROTOCOL_HPP
#define UNIXPROTOCOL_HPP

#ifdef WIN32
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/socket.h>
#endif

namespace moost {
namespace stash {

namespace IP {
  struct TCP;
  struct UDP;
}

namespace local {
  struct datagram;
  struct stream;
}

template<typename Protocol>
class UnixProtocol
{
 public:
  inline int domain() const
  {
    return _domain;
  }

  inline int type() const;

  inline int protocol() const
  {
    return 0;
  }

 protected:
  UnixProtocol(int domain) : _domain(domain) {}

 private:
  UnixProtocol() {}

  int _domain;
};

template<typename Protocol>
class UnixIP: public UnixProtocol<Protocol>
{
 public:
  static UnixIP<Protocol> v4()
  {
    return UnixIP<Protocol>(AF_INET);
  }

  static UnixIP<Protocol> v6()
  {
    return UnixIP<Protocol>(AF_INET6);
  }
 
  inline UnixIP() : UnixProtocol<Protocol>() {}
  inline UnixIP(int domain) : UnixProtocol<Protocol>(domain) {}
  inline ~UnixIP() {}
};

template<>
inline int UnixProtocol<IP::TCP>::type() const
{
  return SOCK_STREAM;
}

template<>
inline int UnixProtocol<IP::UDP>::type() const
{
  return SOCK_DGRAM;
}

#if !defined(WIN32)
template<typename Protocol>
class UnixLocal: public UnixProtocol<Protocol>
{
 public:
  inline UnixLocal() : UnixProtocol<Protocol>(AF_UNIX) {}
  inline ~UnixLocal() {}
};

#else

template <typename Protocol>
class UnixLocal : public UnixProtocol<Protocol>
{
public:
	inline UnixLocal() : UnixProtocol<Protocol>(AF_INET) {}
	inline ~UnixLocal() {}
};

#endif

template<>
inline int UnixProtocol<local::stream>::type() const
{
  return SOCK_STREAM;
}

template<>
inline int UnixProtocol<local::datagram>::type() const
{
  return SOCK_DGRAM;
}

} // stash
} // moost

#endif /* !UNIXPROTOCOL_HPP */
