#ifndef UNIXIP_HPP
#define UNIXIP_HPP

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

template<typename Protocol>
class UnixIP
{
 public:
  static UnixIP<Protocol> v4()
  {
    return UnixIP<Protocol>(PF_INET);
  }

  static UnixIP<Protocol> v6()
  {
    return UnixIP<Protocol>(PF_INET6);
  }

  inline int domain() const
  {
    return _domain;
  }

  inline int type() const;

  inline int protocol() const
  {
    return 0;
  }

 private:
  UnixIP(int domain) : _domain(domain) {}

  int _domain;
};

template<>
inline int UnixIP<IP::TCP>::type() const
{
  return SOCK_STREAM;
}

template<>
inline int UnixIP<IP::UDP>::type() const
{
  return SOCK_DGRAM;
}

} // stash
} // moost

#endif /* !UNIXIP_HPP */
