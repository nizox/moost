#ifndef UNIXSOCKETPAIR_HPP
#define UNIXSOCKETPAIR_HPP

#include "moost/utils.hpp"

#if defined(WIN32)
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/socket.h>
#endif

namespace moost {
namespace stash {

#if !defined(WIN32)

template <typename Protocol, typename Dispatcher>
struct UnixSocketPair
{
  void operator()(UnixSocket<Protocol, Dispatcher> &s1,
                  UnixSocket<Protocol, Dispatcher> &s2,
                  Error &err)
  {
    int v[2];
    Protocol p;

    if (socketpair(p.domain(), p.type(), p.protocol(), v) == -1)
    {
      err.assign(errno);
      return ;
    }

    s1.assign(v[0], err);
    if (err)
    {
      close(v[0]);
      close(v[1]);
      return ;
    }
    s2.assign(v[1], err);
    if (err)
    {
      s1.close(err);
      close(v[1]);
      return ;
    }
  }
};

#else

template <typename Protocol, typename Dispatcher>
struct UnixSocketPair
{
  void operator()(UnixSocket<Protocol, Dispatcher> &s1,
                  UnixSocket<Protocol, Dispatcher> &s2,
                  Error &err)
  {
	UnixLocal<Protocol> p;
    UnixEndpoint<Protocol> ep1("127.0.0.1", 0);
    UnixEndpoint<Protocol> ep2("127.0.0.1", 0);

	s1.open(p, err);
	s2.open(p, err);

    s1.bind(ep1, err);
    s2.bind(ep2, err);
	ep1 = s1.local_endpoint(err);
	ep2 = s2.local_endpoint(err);
    s1.connect(ep2, err);
    s2.connect(ep1, err);
  }
};

#endif

} // !stash
} // !moost

#endif // !UNIXSOCKETPAIR_HPP
