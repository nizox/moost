#ifndef UNIXTIMER_HPP
#define UNIXTIMER_HPP

#include "moost/stash/unix/UnixTimer.h"

namespace moost {
namespace stash {


template<typename Dispatcher>
UnixTimer<Dispatcher>::UnixTimer(Dispatcher &dis)
: _dispatcher(dis), _expected_time(), _tid(-1)
{
  ::gettimeofday(&_expected_time, NULL);
}

template<typename Dispatcher>
UnixTimer<Dispatcher>::UnixTimer(Dispatcher &dis, long e)
      : _dispatcher(dis), _expected_time()
{
  _expected_time.tv_sec = e;
  _expected_time.tv_usec = 0;
}

template<typename Dispatcher>
UnixTimer<Dispatcher>::~UnixTimer()
{
  if (_tid != -1)
  {
    Error err;

    cancel(err);
  }
}

template<typename Dispatcher>
void UnixTimer<Dispatcher>::wait(Error &err)
{
  timeval now;
  timeval diff;
  long useconds;
  int ret;
 
  gettimeofday(&now, NULL);
  timersub(&_expected_time, &now, &diff);
  useconds = diff.tv_sec * 1000000 + diff.tv_usec;
  ret = usleep(useconds);
  if (ret == -1)
    err.assign(errno);
}

template<typename Dispatcher>
template<typename WaitHandler>
void UnixTimer<Dispatcher>::async_wait(WaitHandler cb)
{
  Error err;
  SPIOperation op(new WaitOperation<WaitHandler>(cb));

  _tid = _dispatcher.register_event(Dispatcher::TimerEventType::TM_EXPIRE, _expected_time, op, err);
}

// TODO
template<typename Dispatcher>
void UnixTimer<Dispatcher>::expires_in(long ms, Error &)
{
  timeval add = { ms / 1000, (ms % 1000) * 1000 };
  timeval tmp;

  timeradd(&_expected_time, &add, &tmp);
  _expected_time = tmp;
}

template<typename Dispatcher>
void UnixTimer<Dispatcher>::expires_at(long s, Error &)
{
  _expected_time.tv_sec = s;
  _expected_time.tv_usec = 0;
}

template<typename Dispatcher>
timeval const &UnixTimer<Dispatcher>::expected_time() const
{
  return _expected_time;
}

template<typename Dispatcher>
void UnixTimer<Dispatcher>::cancel(Error &err)
{
  if (!err)
  {
    _dispatcher.cancel_event(Dispatcher::TimerEventType::TM_EXPIRE, _tid, err);
    _tid = -1;
  }
}

} // stash
} // moost

#endif /* !UNIXTIMER_HPP */
