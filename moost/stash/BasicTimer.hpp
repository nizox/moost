#ifndef BASICTIMER_HPP
#define BASICTIMER_HPP

#include "moost/stash/BasicDispatcher.hpp"
#include "moost/utils/Error.hpp"

namespace moost {
namespace stash {

template<typename Implementation, typename DispatcherImplementation>
class BasicTimer
{
 public:
  inline BasicTimer(BasicDispatcher<DispatcherImplementation> &dispatcher)
      : _implem(dispatcher._implem) {}

  inline BasicTimer(BasicDispatcher<DispatcherImplementation> &dispatcher,
             long ms)
      : _implem(dispatcher._implem, ms) {}

  inline ~BasicTimer() {}

  inline void wait(Error &err)
  {
    _implem.wait(err);
  }

  template<typename WaitHandler>
  inline void async_wait(WaitHandler cb)
  {
    _implem.async_wait(cb);
  }

  inline void expires_in(long ms, Error &err)
  {
    _implem.expires_in(ms, err);
  }

  inline void expires_at(long s, Error &err)
  {
    _implem.expires_at(s, err);
  }

  inline void cancel(Error &err)
  {
    _implem.cancel(err);
  }

 private:
  Implementation _implem;
};

} // stash
} // moost

#endif /* !BASICTIMER_HPP */
