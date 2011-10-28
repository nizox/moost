#ifndef BOOSTSOCKETACCEPTOR_H
#define BOOSTSOCKETACCEPTOR_H

#include "moost/utils/Error.hpp"
#include "moost/stash/boost/BoostIP.hpp"
#include "moost/stash/boost/BoostDispatcher.h"
#include "moost/stash/boost/BoostSocket.hpp"
#include "moost/stash/boost/boost_meta_handle.hpp"
#include "moost/stash/boost/boost_utils.hpp"

namespace moost {
namespace stash {

template <typename Protocol>
class BoostSocketAcceptor
{
 public:
  typedef typename meta_handle<Protocol>::TYPE meta_type;

  typedef typename meta_type::acceptor::reuse_address ReuseAddress;

  BoostSocketAcceptor(BoostDispatcher &);
  BoostSocketAcceptor(BoostDispatcher &, BoostIP<Protocol> const &);
  BoostSocketAcceptor(BoostDispatcher &, BoostEndpoint<Protocol> const &, ReuseAddress reuse);
  ~BoostSocketAcceptor();

  template<typename Option>
  Option get_option(Option, Error &);
  template<typename Option>
  void set_option(Option, Error &);

  void open(BoostIP<Protocol> const &, Error &);
  void bind(BoostEndpoint<Protocol> const &, Error &);
  void close(Error &);
  void cancel(Error &);
  void listen(int, Error &);
  void accept(BoostSocket<Protocol> &, BoostEndpoint<Protocol> &, Error &);
  template<typename AcceptHandler>
  void async_accept(BoostSocket<Protocol> &, BoostEndpoint<Protocol> &, AcceptHandler);

 private:
  typename meta_type::acceptor _handle;

  template<typename AcceptHandler>
  static void _async_accept_handle(AcceptHandler cb, boost::system::error_code const &berr)
  {
    Error err = moost_error(berr);

    cb(err);
  }
};

} // stash
} // moost



#endif /* !BOOSTSOCKETACCEPTOR_H */
