#ifndef KQUEUEREACTOR_HPP__
#define KQUEUEREACTOR_HPP__

#include "moost/stash/unix/UnixDispatcher.hpp"

#include <sys/event.h>

#include <iostream>
#include <stdexcept>
#include <map>

namespace moost {
namespace stash {

class KQueueReactor
{
 public:
  enum IOEventType : int
  {
    EV_IN  = ~EVFILT_READ + 1,
    EV_OUT = ~EVFILT_WRITE + 1,
  };

  enum TimerEventType : int
  {
    TM_EXPIRE = EVFILT_TIMER,
  };

  inline KQueueReactor() : _kfd(kqueue()), _io_event_map() {}
  inline ~KQueueReactor() 
  {
    close(_kfd);
  }

  template<typename EventType, typename Argument>
  inline int register_event(EventType, Argument const &, SPIOperation const &, Error &)
  {
    throw std::logic_error("Register event method is not implemented");
    return -1;
  }

  template <typename EventType>
  inline void cancel_event(EventType, int, Error &)
  {
    throw std::logic_error("Cancel events method is not implemented");
  }

  inline void _kevent(int fd, IOEventType ev, int action, int flags, void *udata, Error &err)
  {
    struct timespec ts = { 0, 0 };
    struct kevent kev;

    EV_SET(&kev, fd, ~ev + 1, action, flags, 0, udata);
    if (kevent(_kfd, &kev, 1, NULL, 0, &ts) == -1)
      err.assign(errno);
    //std::cout << "kevent " << ev << " " << action << std::endl;
  }

  void run(Error &, bool run = false);

 private:
  int _kfd;
  std::map<int, event<IOEventType>> _io_event_map;
};


template<>
inline int KQueueReactor::register_event<KQueueReactor::IOEventType, int>(
    IOEventType ev,
    int const &fd,
    SPIOperation const &op,
    Error &err)
{
  std::map<int, event<IOEventType>>::iterator it = _io_event_map.find(fd);
  bool had;

  if (it == _io_event_map.end())
    it = _io_event_map.insert(it, std::pair<int, event<IOEventType>>(fd, event<IOEventType>(fd)));

  //std::cout << "register event " << ev << " on " << fd << std::endl;
  had = it->second.has(ev);
  it->second.push(ev, op);

  if (had == false)
    _kevent(fd, ev, EV_ADD | EV_CLEAR, 0, &it->second, err);
  return fd;
}

template <>
inline void KQueueReactor::cancel_event<KQueueReactor::IOEventType>(KQueueReactor::IOEventType ev, int id, Error &err)
{
  std::map<int, event<IOEventType>>::iterator it = _io_event_map.find(id);

  if (it != _io_event_map.end())
  {
    bool had = it->second.has(ev);
    it->second.flush(ev);

    if (had)
      _kevent(id, ev, EV_DELETE, 0, NULL, err);
  }
}
/*
template<>
inline int KQueueReactor::register_event<KQueueReactor::TimerEventType, timeval>(
    TimerEventType,
    timeval const &ms,
    SPIOperation const &op,
    Error &err)
{
  return -1;
}

template <>
inline void KQueueReactor::cancel_event<KQueueReactor::TimerEventType>(KQueueReactor::TimerEventType, int tid, Error &err)
{
}
*/


inline void KQueueReactor::run(Error &err, bool run)
{
  struct kevent kev[128];

  while (!err && (!_io_event_map.empty() || run))
  {
    //std::cout << "run wait" << std::endl;
    int ret = kevent(_kfd, NULL, 0, kev, sizeof(kev) / sizeof(*kev), NULL);
    //std::cout << "  done " << ret << std::endl;

    if (ret == -1 && errno != EINTR)
    {
      err.assign(errno);
      break;
    }

    for (int i = 0; i < ret; ++i)
    {
      event<IOEventType> *evptr = static_cast<event<IOEventType> *>(kev[i].udata);
      IOEventType ev = static_cast<IOEventType>(~kev[i].filter + 1);

      //std::cout << "handle event " << ev << " on " << evptr->id() << std::endl;
      evptr->handle(ev);
    }
  }
}

} // stash
} // moost

#endif
