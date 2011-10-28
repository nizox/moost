#ifndef BOOSTIP_HPP
#define BOOSTIP_HPP

#include <boost/asio.hpp>

#include "moost/stash/boost/boost_meta_handle.hpp"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostIP
{
 public:
  typedef typename meta_handle<Protocol>::TYPE meta_type;

  static BoostIP<Protocol> v4()
  {
    return BoostIP<Protocol>(meta_type::v4());
  }

  static BoostIP<Protocol> v6()
  {
    return BoostIP<Protocol>(meta_type::v6());
  }

  inline int domain()
  {
    return _handle.family();
  }

  inline int type()
  {
    return _handle.type();
  }

  inline int protocol()
  {
    return _handle.protocol();
  }

 private:
  BoostIP(meta_type const &handle) : _handle(handle) {}

  meta_type _handle;

  template<typename T> friend class BoostEndpoint;
  template<typename T> friend class BoostSocket;
  template<typename T> friend class BoostSocketAcceptor;
};

} // stash
} // moost

#endif /* !BOOSTIP_HPP */
