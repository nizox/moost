#ifndef UNIXTIMER_H
#define UNIXTIMER_H

#include "moost/utils/Error.hpp"
#include "moost/stash/unix/UnixDispatcher.hpp"

#include <cerrno>
#include <ctime>
#include <ctime>

#ifdef WIN32
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/time.h>
#endif

namespace moost {
namespace stash {

template<typename WaitHandler>
class WaitOperation: public virtual IOperation
{
 public:
  WaitOperation(WaitHandler cb) : _cb(cb) {}
  virtual ~WaitOperation() {}

  virtual int operator()()
  {
    Error err;

    _cb(err);
    return (0);
  }

 private:
  WaitHandler _cb;
};


template <typename Dispatcher>
class UnixTimer
{
 public:
  UnixTimer(Dispatcher &dis);
  UnixTimer(Dispatcher &dis, long e);
  ~UnixTimer();

  void wait(Error &err);

  template<typename WaitHandler>
  void async_wait(WaitHandler cb);
  void expires_in(long ms, Error &);
  void expires_at(long s, Error &);
  timeval const &expected_time() const;

  void cancel(Error &);

 private:
  Dispatcher &_dispatcher;
  timeval _expected_time;
  int _tid;
};

} // stash
} // moost

#endif /* !UNIXTIMER_H */
