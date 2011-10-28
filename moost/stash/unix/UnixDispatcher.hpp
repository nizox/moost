#ifndef UNIXDISPATCHER_HPP
#define UNIXDISPATCHER_HPP

#include "moost/utils/Error.hpp"

namespace moost {
namespace stash {

template <typename Reactor>
class UnixDispatcher
{
 public:
  typedef typename Reactor::IOEventType IOEventType;
  typedef typename Reactor::TimerEventType TimerEventType;

  inline UnixDispatcher() : _reactor() {}
  inline ~UnixDispatcher() {}

  template <typename EventType, typename Argument>
  inline int register_event(EventType ev, Argument const &arg,
                            SPIOperation const &op, Error &err)
  {
    return _reactor.register_event<EventType, Argument>(ev, arg, op, err);
  }

  template <typename EventType>
  inline void cancel_event(EventType ev, int id, Error &err)
  {
    return _reactor.cancel_event<EventType>(ev, id, err);
  }

  inline void run(Error &err, bool run = false)
  {
    _reactor.run(err, run);
  }

 private:
  Reactor _reactor;
};

} // stash
} // moost

#endif // !UNIXDISPATCHER_HPP
