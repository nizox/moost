#ifndef SELECTREACTOR_HPP__
#define SELECTREACTOR_HPP__

#include "moost/stash/Buffer.hpp"
#include "moost/stash/unix/UnixEndpoint.hpp"
#include "moost/stash/unix/UnixSocket.h"
#include "moost/stash/unix/UnixDispatcher.hpp"
#include "moost/threads.hpp"
#include "moost/utils.hpp"
#include "moost/singleton.hpp"

#include <stdexcept>
#include <iostream>
#include <map>
#include <list>

#if defined(WIN32)
#include "moost/stash/unix/utils.hpp"
#else
#include <sys/select.h>
#endif

namespace moost {
namespace stash {

#if defined(WIN32)

class _WSAInitialization: public moost::singleton<_WSAInitialization>
{
 public:
  Mutex _lock;

  inline _WSAInitialization()
  {
    ScopedMutex l(_lock);

    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
  }

  inline ~_WSAInitialization()
  {
    ScopedMutex l(_lock);

    WSACleanup();
  }
};

#endif


class SelectReactor
{
 public:
  enum IOEventType {
    EV_IN  = (1 << 0),
    EV_OUT = (1 << 1),
  };

  enum TimerEventType {
    TM_EXPIRE = (1 << 0),
  };

  inline SelectReactor() : _i(0)
  {
#if defined(WINDOWS)
    _WSAInitialization::get_instance();
#endif
  }

  inline ~SelectReactor() {}

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
    int nready;
    int fdmax;
    fd_set readfds;
    fd_set writefds;
    fd_set errorfds;

    while (!err && (!_io_event_map.empty() || !_timer_list.empty() || run))
    {
      struct timeval *timeout = NULL;
      struct timeval waitval;
      bool less = true;

      if (!_timer_list.empty())
      {
        struct timeval now;

        gettimeofday(&now, NULL);
        if (timercmp(&_timer_list.front().first, &now, >=))
        {
          timersub(&_timer_list.front().first, &now, &waitval);
          timeout = &waitval;
        }
      }

      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
      FD_ZERO(&errorfds);

      fdmax = -1;
      for (std::map<int, event<IOEventType>>::const_iterator cit = _io_event_map.begin();
           cit != _io_event_map.end();
           ++cit)
      {
        int e = cit->second.events();

        if (cit->first > fdmax)
          fdmax = cit->first;

        if (e & EV_IN)
          FD_SET(cit->first, &readfds);
        if (e & EV_OUT)
          FD_SET(cit->first, &writefds);
        // Always watch for errors
        FD_SET(cit->first, &errorfds);

        //std::cout << (*cit).first << " " << FD_ISSET((*cit).first, &readfds) << " " << FD_ISSET((*cit).first, &writefds) << std::endl;
      }

      nready = select(fdmax + 1, &readfds, &writefds, &errorfds, timeout);
      if (nready == -1)
        err.assign(nready);

      //std::cout << "nready " << nready << std::endl;

      // determine if there are timer operation to execute
      while (!_timer_list.empty() && less)
      {
        struct timeval now;
        std::pair<struct timeval, SPIOperation> p = _timer_list.front();

        gettimeofday(&now, NULL);
        less = timercmp(&p.first, &now, <);
        if (less)
        {
          //std::cout << "less " << std::endl;
          _timer_list.pop_front();
          (*p.second)();
        }
        else
        {
          timersub(&p.first, &now, &waitval);
          timeout = &waitval;
        }
      }

      std::map<int, event<IOEventType>>::iterator it = _io_event_map.begin();
      while (nready > 0 && it != _io_event_map.end())
      {
        if (FD_ISSET(it->first, &readfds) || FD_ISSET(it->first, &errorfds))
        {
          // watching for reading operation
          it->second.handle(EV_IN);
          --nready;
        }

        if (FD_ISSET(it->first, &writefds) || FD_ISSET(it->first, &errorfds))
        {
          // watching for writing operation
          it->second.handle(EV_OUT);
          --nready;
        }

        if (it->second.events() == 0)
        {
          std::map<int, event<IOEventType>>::iterator tmpit = it++;
          _io_event_map.erase(tmpit);
        }
        else
          ++it;
      }
    }
  }

 private:
  int _i;
  std::map<int, struct timeval> _timer_id_assoc;
  std::map<int, event<IOEventType>> _io_event_map;
  std::list<std::pair<struct timeval, SPIOperation>> _timer_list;
};

template<>
inline int SelectReactor::register_event<SelectReactor::IOEventType, int>(
    IOEventType ev,
    int const &fd,
    SPIOperation const &op,
    Error &)
{
  std::map<int, event<IOEventType>>::iterator it = _io_event_map.find(fd);

  if (it == _io_event_map.end())
    it = _io_event_map.insert(it, std::make_pair(fd, event<IOEventType>(fd)));

  //std::cout << "register event " << ev << " on " << fd << std::endl;
  it->second.push(ev, op);

  return fd;
}

template <>
inline void SelectReactor::cancel_event<SelectReactor::IOEventType>(IOEventType ev, int id, Error &)
{
  std::map<int, event<IOEventType>>::iterator it = _io_event_map.find(id);

  if (it != _io_event_map.end())
    it->second.flush(ev);
}

template<>
inline int SelectReactor::register_event<SelectReactor::TimerEventType, struct timeval>(
    TimerEventType, 
    struct timeval const &tv,
    SPIOperation const &op,
    Error &)
{
  std::list<std::pair<struct timeval, SPIOperation>>::iterator it = _timer_list.begin();
  std::list<std::pair<struct timeval, SPIOperation>>::iterator ite = _timer_list.end();

  while (it != ite && timercmp(&tv, &it->first, <))
    it++;

  _timer_list.insert(it, std::make_pair(tv, op));
  _timer_id_assoc[_i] = tv;

  return _i++;
}

template <>
inline void SelectReactor::cancel_event<SelectReactor::TimerEventType>(TimerEventType, int id, Error &)
{
  struct timeval tv = _timer_id_assoc[id];
  std::list<std::pair<struct timeval, SPIOperation>>::iterator it = _timer_list.begin();
  std::list<std::pair<struct timeval, SPIOperation>>::iterator ite = _timer_list.end();

  while (it != ite && timercmp(&tv, &it->first, !=))
    it++;

  if (it != ite)
    _timer_list.erase(it);
}


} // stash
} // moost

#endif
