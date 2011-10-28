#ifndef BASICDISPATCHER_HPP
#define BASICDISPATCHER_HPP

#include "moost/utils/Error.hpp"

#include <iostream>
#include <algorithm>
#include <map>
#include <list>
#include <vector>

#ifdef WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

namespace moost {
namespace stash {

class IOperation
{
  public:
    inline virtual ~IOperation() {}

    virtual int operator()() = 0;
};
typedef std::tr1::shared_ptr<IOperation> SPIOperation;


template<typename EventType>
class event: public std::tr1::enable_shared_from_this<event<EventType>>
{
 public:
  typedef EventType Type;

  event() : _id(-1), _events(0), _ops() {}
  event(int id) : _id(id), _events(0), _ops() {}
  ~event() {}

  inline bool operator<(event<EventType> const &ev) const
  {
    return _id < ev._id;
  }

  inline int id() const
  {
    return _id;
  }

  inline int events() const
  {
    return _events;
  }

  inline bool has(EventType type) const
  {
    return _events & type;
  }

  inline void handle(EventType type)
  {
    typename std::map<EventType, std::list<SPIOperation>>::iterator it;

    it = _ops.find(type);
    if (it != _ops.end())
    {
      while (!it->second.empty())
      {
        SPIOperation op(it->second.front());

        it->second.pop_front();
        if ((*op)() == 0)
        {
          it->second.push_front(op);
          break;
        }
      }
      if (it->second.empty())
        _events &= ~type;
    }
  }

  inline void flush(EventType type)
  {
    typename std::map<EventType, std::list<SPIOperation>>::iterator it;

    it = _ops.find(type);
    if (it != _ops.end())
      it->second.clear();
    _events &= ~type;
  }

  inline void push(EventType type, SPIOperation const &op)
  {
    _ops[type].push_back(op);
    _events |= type;
  }

 private:
  int _id;
  int _events;
  std::map<EventType, std::list<SPIOperation>> _ops;
};


template <typename Implementation>
class BasicDispatcher
{
 public:
  typedef typename Implementation::IOEventType IOEventType;
  typedef typename Implementation::TimerEventType TimerEventType;

  template <typename EventType, typename Argument>
  inline int register_event(EventType ev, Argument const &arg,
                            SPIOperation const &op, Error &err)
  {
    return _implem.register_event<EventType, Argument>(ev, arg, op, err);
  }

  template <typename EventType>
  inline void cancel_event(EventType ev, int id, Error &err)
  {
    return _implem.cancel_event<EventType>(ev, id, err);
  }

  inline void run(Error &err, bool run = false)
  {
    _implem.run(err, run);
  }

 private:
  Implementation _implem;

  template <typename P, typename I, typename DI> friend class BasicSocket;
  template <typename P, typename I, typename DI> friend class BasicSocketAcceptor;
  template <typename P, typename I, typename DI> friend class BasicResolver;
  template <typename I, typename DI> friend class BasicTimer;
};

} // stash
} // moost


#endif /* !BASICDISPATCHER_HPP */
