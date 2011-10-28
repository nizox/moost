#ifndef LINUXREACTOR_HPP__
#define LINUXREACTOR_HPP__

#include "moost/stash/unix/UnixDispatcher.hpp"
#include "moost/stash/unix/EPoll.hpp"

#include <sys/timerfd.h>

#include <iostream>
#include <stdexcept>
#include <map>

namespace moost {
namespace stash {

class LinuxTimerOperation: public IOperation
{
 public:
  inline LinuxTimerOperation(int fd, SPIOperation const &op)
      : _fd(fd), _op(op) {}
  inline virtual ~LinuxTimerOperation() {}

  inline virtual int operator()()
  {
    // TODO read timerfd
    (*_op)();
    return (0);
  }

 private:
  int _fd;
  SPIOperation _op;
};


class LinuxReactor
{
 public:
  typedef EPoll::EventType IOEventType;

  enum TimerEventType {
    TM_EXPIRE = (1 << 0),
  };

  inline LinuxReactor() : _epoll(), _io_event_map() {}
  inline ~LinuxReactor() {}

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

  inline void run(Error &err, bool run)
  {
    while (!err && (!_io_event_map.empty() || run))
    {
      // std::cout << std::dec;
      // std::cout << "[" << ::syscall(SYS_gettid) << "] " << "run wait" << std::endl;
      EPoll::iterator it = _epoll.poll(err);
      // std::cout << "[" << ::syscall(SYS_gettid) << "] " << "  done" << std::endl;

      while (!it.empty())
      {
        IOEventType types[] = { IOEventType::EV_IN, IOEventType::EV_OUT };
        EPoll::Event *evptr = static_cast<EPoll::Event *>(it->second);
        EPoll::SPEvent ev(evptr->shared_from_this());

        // std::cout << "[" << ::syscall(SYS_gettid) << "] " << "  handling fd " <<
        //    ev->id() << " " << std::hex << it->first << std::dec << std::endl;

        for (std::size_t i = 0; i < sizeof(types) / sizeof(*types); ++i)
        {
          if (it->first & types[i])
            ev->handle(types[i]);
        }

        if (ev->events() == 0)
        {
          Error terr;

          _epoll.remove(ev->id(), terr);
          if (terr)
            if (terr != EBADF && terr != ENOENT && terr != EPERM) // OK fd was closed before epoll_ctl call
              err = terr;
        }

        ++it;
      }
    }
  }

 private:
  EPoll _epoll;
  std::map<int, EPoll::SPEvent> _io_event_map;
};


template<>
inline int LinuxReactor::register_event<LinuxReactor::IOEventType, int>(
    IOEventType evtype,
    int const &fd,
    SPIOperation const &op,
    Error &err)
{
  std::map<int, EPoll::SPEvent>::iterator it = _io_event_map.find(fd);

  if (it == _io_event_map.end())
  {
    EPoll::SPEvent spev(new EPoll::Event(fd));

    // std::cout << "registering " << fd << std::endl;
    it = _io_event_map.insert(it, std::pair<int, EPoll::SPEvent>(fd, spev));
    it->second->push(evtype, op);
    _epoll.add(fd, IOEventType::EV_IN | IOEventType::EV_OUT, static_cast<void *>(spev.get()), err);
  }
  else
  {
    int had = it->second->events();

    it->second->push(evtype, op);
    if (had == 0)
      _epoll.add(fd, IOEventType::EV_IN | IOEventType::EV_OUT, static_cast<void *>(it->second.get()), err);
  }
  return fd;
}

template <>
inline void LinuxReactor::cancel_event<LinuxReactor::IOEventType>(LinuxReactor::IOEventType ev, int id, Error &)
{
  std::map<int, EPoll::SPEvent>::iterator it = _io_event_map.find(id);

  if (it != _io_event_map.end())
    it->second->flush(ev);
}

template<>
inline int LinuxReactor::register_event<LinuxReactor::TimerEventType, timeval>(
    TimerEventType,
    timeval const &ms,
    SPIOperation const &op,
    Error &err)
{
  struct itimerspec it;
  int tid = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC | TFD_NONBLOCK);

  if (tid == -1)
  {
    err.assign(errno);
    return -1;
  }

  it.it_value.tv_sec = ms.tv_sec;
  it.it_value.tv_nsec = ms.tv_usec * 1000;

  it.it_interval.tv_sec = 0;
  it.it_interval.tv_nsec = 0;

  if (timerfd_settime(tid, TFD_TIMER_ABSTIME, &it, NULL) == -1)
  {
    err.assign(errno);
    close(tid);
    return -1;
  }

  SPIOperation top(new LinuxTimerOperation(tid, op));
  register_event(IOEventType::EV_IN, tid, top, err);
  if (err)
  {
    close(tid);
    return -1;
  }

  return tid;
}

template <>
inline void LinuxReactor::cancel_event<LinuxReactor::TimerEventType>(LinuxReactor::TimerEventType, int tid, Error &err)
{
  cancel_event<IOEventType>(IOEventType::EV_IN, tid, err);
  if (close(tid) == -1)
    err.assign(errno);
}

} // stash
} // moost

#endif
