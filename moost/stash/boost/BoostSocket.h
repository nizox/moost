#ifndef BOOSTSOCKET_H
#define BOOSTSOCKET_H

#include <iostream>

#include "moost/utils/Error.hpp"
#include "moost/stash/boost/BoostIP.hpp"
#include "moost/stash/boost/BoostDispatcher.h"
#include "moost/stash/boost/BoostEndpoint.hpp"
#include "moost/stash/boost/boost_meta_handle.hpp"
#include "moost/stash/boost/boost_utils.hpp"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostSocket
{
 public:
  typedef typename meta_handle<Protocol>::TYPE meta_type;

  BoostSocket(BoostDispatcher &);
  BoostSocket(BoostDispatcher &, BoostIP<Protocol> const &);
  BoostSocket(BoostDispatcher &, BoostEndpoint<Protocol> const &);
  ~BoostSocket();

  void open(BoostIP<Protocol> const &, Error &);
  template<typename ConnectHandler>
  void async_connect(BoostEndpoint<Protocol> const &, ConnectHandler);
  void bind(BoostEndpoint<Protocol> const &, Error &);
  void close(Error &);
  void connect(BoostEndpoint<Protocol> const &, Error &);
  void cancel(Error &);

 protected:
  typename meta_type::socket _handle;

 private:
  template<typename ConnectHandler>
  static void _async_connect_handle(ConnectHandler cb, boost::system::error_code const &berr)
  {
    Error err = moost_error(berr);

    cb(err);
  }

  template <typename P> friend class BoostSocketAcceptor;
};

} // stash
} // moost

#endif /* !BOOSTSOCKET_H */
