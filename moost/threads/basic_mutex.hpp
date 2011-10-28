#ifndef BASIC_MUTEX_35VRTAJR

#define BASIC_MUTEX_35VRTAJR

#include "moost/threads/MutexTraits.h"

namespace moost {
namespace cond { template <typename implem> class basic_cond; }
namespace mutex {

template <typename Implem>
class basic_mutex
{
 public:
  inline void lock()
  {
    _implem.lock();
  }

  inline void unlock()
  {
    _implem.unlock();
  }

  inline void try_lock()
  {
    _implem.try_lock();
  }
 private:
  template <typename T> friend class moost::cond::basic_cond;
  Implem _implem;
};

}

typedef mutex::basic_mutex<mutex::MutexTraits::MutexImplem> Mutex;

class ScopedMutex
{
 public:
  inline ScopedMutex(Mutex &mutex) : _mutex(mutex)
  {
    _mutex.lock();
  }
  inline ~ScopedMutex()
  {
    _mutex.unlock();
  }

 private:
  Mutex &_mutex;
};

}

#endif /* end of include guard: BASIC_MUTEX_35VRTAJR */
