#include <boost/bind.hpp>
#include <boost/date_time.hpp>

#include "moost/stash/boost/BoostTimer.h"

namespace moost {
namespace stash {

BoostTimer::BoostTimer(BoostDispatcher &dispatcher)
    : _handle(dispatcher._ioserv)
{
}

BoostTimer::BoostTimer(BoostDispatcher &dispatcher,
                              long ms)
    : _handle(dispatcher._ioserv, boost::posix_time::from_time_t(ms))
{
}

BoostTimer::~BoostTimer()
{
}

void BoostTimer::wait(Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.wait(berr);
  boost2moost_error(err, berr);
}

void BoostTimer::expires_in(long ms, Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.expires_from_now(boost::posix_time::milliseconds(ms), berr);
  boost2moost_error(err, berr);
}

void BoostTimer::expires_at(long s, Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.expires_at(boost::posix_time::from_time_t(s), berr);
  boost2moost_error(err, berr);
}

void BoostTimer::cancel(Error &err)
{
  boost::system::error_code berr = boost_error(err);

  _handle.cancel(berr);
  boost2moost_error(err, berr);
}

} // stash
} // moost
