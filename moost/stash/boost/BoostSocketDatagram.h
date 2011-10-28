#ifndef BOOSTSOCKETDATAGRAM_H
#define BOOSTSOCKETDATAGRAM_H

#include "moost/stash/Buffer.hpp"
#include "moost/stash/boost/boost_meta_handle.hpp"
#include "moost/stash/boost/boost_utils.hpp"
#include "moost/stash/boost/BoostDispatcher.h"
#include "moost/stash/boost/BoostSocket.hpp"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostSocketDatagram: public BoostSocket<Protocol>
{
 public:
  BoostSocketDatagram(BoostDispatcher &);
  BoostSocketDatagram(BoostDispatcher &, BoostIP<Protocol> const &);
  BoostSocketDatagram(BoostDispatcher &, BoostEndpoint<Protocol> const &);
  ~BoostSocketDatagram();

  template<typename ReceiveHandler>
  void async_receive(Buffer &, ReceiveHandler);
  template<typename ReceiveHandler>
  void async_receive_from(Buffer &, BoostEndpoint<Protocol> &, ReceiveHandler);
  template<typename SendHandler>
  void async_send(Buffer const &, SendHandler);
  template<typename SendHandler>
  void async_send_to(Buffer const &, BoostEndpoint<Protocol> const &, SendHandler);
  size_t receive(Buffer &, Error &);
  size_t receive_from(Buffer &, BoostEndpoint<Protocol> &, Error &);
  size_t send(Buffer const &, Error &);
  size_t send_to(Buffer const &, BoostEndpoint<Protocol> const &, Error &);

 private:
  template<typename Handler>
  static void _async_handle(Handler cb, size_t bytes, boost::system::error_code const &berr)
  {
    Error err = moost_error(berr);

    cb(bytes, err);
  }
};

} // stash
} // moost



#endif /* !BOOSTSOCKETDATAGRAM_H */
