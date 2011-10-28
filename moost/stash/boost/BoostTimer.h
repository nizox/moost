#ifndef BOOSTTIMER_H
#define BOOSTTIMER_H

#include <boost/asio.hpp>

#include "moost/utils/Error.hpp"
#include "moost/stash/boost/BoostDispatcher.h"
#include "moost/stash/boost/boost_utils.hpp"

namespace moost {
namespace stash {

class BoostTimer
{
 public:

  BoostTimer(BoostDispatcher &);
  BoostTimer(BoostDispatcher &, long);
  ~BoostTimer();

  void wait(Error &);
  template<typename WaitHandler>
  void async_wait(WaitHandler cb)
  {
    _handle.async_wait(boost::bind(BoostTimer::_async_handle<WaitHandler>, cb, boost::asio::placeholders::error));
  }
  void expires_in(long, Error &);
  void expires_at(long, Error &);
  void cancel(Error &);

 private:
  typename boost::asio::deadline_timer _handle;

 private:
  template<typename WaitHandler>
  static void _async_handle(WaitHandler cb, boost::system::error_code const &berr)
  {
    Error err = moost_error(berr);

    cb(err);
  }
};

} // stash
} // moost

#endif /* !BOOSTTIMER_H */
