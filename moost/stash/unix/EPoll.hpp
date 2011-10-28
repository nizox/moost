#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "moost/stash/unix/UnixDispatcher.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <utility>

#include <sys/epoll.h>

namespace moost {
namespace stash {

class EPollIterator;

class EPoll
{
 public:
  typedef EPollIterator iterator;

  enum EventType {
    EV_IN  = EPOLLIN,
    EV_OUT = EPOLLOUT,
  };
  typedef event<EventType> Event;
  typedef std::tr1::shared_ptr<Event> SPEvent;

  inline EPoll() : _efd(epoll_create1(0)), _events()
  {
  }

  inline ~EPoll()
  {
    close(_efd);
  }
  inline void add(int fd, int ev, void *ptr, Error &err)
  {
    _apply(fd, EPOLL_CTL_ADD, ev, ptr, err);
  }

  inline void modify(int fd, int ev, void *ptr, Error &err)
  {
    _apply(fd, EPOLL_CTL_MOD, ev, ptr, err);
  }

  inline void remove(int fd, Error &err)
  {
    int ret;

    // std::cout << "epoll del " << fd << std::endl;
    ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, 0);
    if (ret == -1)
      err.assign(errno);
  }

  iterator poll(Error &err);
 private:
  inline void _apply(int fd, int mode, int ev, void *ptr, Error &err)
  {
    int ret;
    epoll_event tmp = { 0, { 0 } };
   
    tmp.events = ev | EPOLLERR | EPOLLHUP | EPOLLET;
    tmp.data.ptr = ptr;

    /*if (mode == EPOLL_CTL_ADD)
      // std::cout << "epoll add " << fd << std::endl;
    else if (mode == EPOLL_CTL_MOD)
      // std::cout << "epoll mod " << fd << std::endl;*/
    ret = epoll_ctl(_efd, mode, fd, &tmp);
    if (ret == -1)
      err.assign(errno);
  }

  enum { max_events = 1024 };

  int _efd;
  epoll_event _events[max_events];
};

class EPollIterator
{
 public:
  inline EPollIterator(EPollIterator const &it)
  {
    *this = it;
  }

  inline EPollIterator &operator=(EPollIterator const &it)
  {
    if (this != &it)
    {
      _events = it._events;
      _i_events = it._n_events;
      _n_events = it._n_events;
      _tmp_event = it._tmp_event;
    }
    return *this;
  }

  inline ~EPollIterator() {}

  std::pair<int, void *> &operator*()
  {
    return _tmp_event;
  }

  std::pair<int, void *> *operator->()
  {
    return &_tmp_event;
  }

  EPollIterator &operator++()
  {
    ++_i_events;
    if (_i_events < _n_events)
      _make_tmp();
    return *this;
  }

  bool empty() const
  {
    if (_i_events < _n_events)
      return false;
    return true;
  }

 private:
  inline EPollIterator()
      : _events(0), _i_events(0), _n_events(0), _tmp_event() {}
  inline EPollIterator(epoll_event *events, std::size_t n)
      : _events(events), _i_events(0), _n_events(n), _tmp_event()
  {
    if (_n_events > 0)
      _make_tmp();
  }

  void _make_tmp()
  {
    int ev = _events[_i_events].events;
    void *ptr = _events[_i_events].data.ptr;

    _tmp_event = std::pair<int, void *>(ev, ptr);
  }

  epoll_event *_events;
  std::size_t _i_events;
  std::size_t _n_events;
  std::pair<int, void *> _tmp_event;

  friend class EPoll;
};

inline EPoll::iterator EPoll::poll(Error &err)
{
  int nready;

  nready = epoll_wait(_efd, _events, max_events, -1);
  if (nready == -1)
  {
    if (errno != EINTR)
      err.assign(errno);
    return iterator();
  }
  return iterator(_events, nready);
}


} // stash
} // moost

#endif // !EPOLL_HPP
