#ifndef BOOSTSOCKETSTREAM_H
#define BOOSTSOCKETSTREAM_H

#include "moost/stash/Buffer.hpp"
#include "moost/stash/boost/BoostSocket.hpp"
#include "moost/stash/boost/boost_utils.hpp"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostSocketStream: public BoostSocket<Protocol>
{
 public:
  BoostSocketStream(BoostDispatcher &);
  BoostSocketStream(BoostDispatcher &, BoostIP<Protocol> const &);
  BoostSocketStream(BoostDispatcher &, BoostEndpoint<Protocol> const &);
  ~BoostSocketStream();

  template<typename ReceiveHandler>
  void async_receive(Buffer &, ReceiveHandler);
  template<typename ReceiveHandler>
  void async_receive_until(moost::streambuf &, std::string const &, ReceiveHandler);
  template<typename SendHandler>
  void async_send(Buffer const &, SendHandler);
  size_t receive(Buffer &, Error &);
  size_t send(Buffer const &, Error &);

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



#endif /* !BOOSTSOCKETSTREAM_H */
