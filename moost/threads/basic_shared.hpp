#ifndef BASIC_SHARED_Z7A1MWF

#define BASIC_SHARED_Z7A1MWF


#include "SharedTraits.h"

namespace moost {
namespace shared_lock {

template <typename Implem>
class basic_shared_lock
{
 public:
  basic_shared_lock(){};
  ~basic_shared_lock(){};

  void read_lock()
  {
    _implem.read_lock();
  }

  void try_read_lock()
  {
    _implem.try_read_lock();
  }

  void write_lock()
  {
    _implem.write_lock();
  }

  void try_write_lock()
  {
    _implem.try_write_lock();
  }

  void unlock()
  {
    _implem.unlock();
  }
 private:
  Implem _implem;
};
}
typedef shared_lock::basic_shared_lock<SharedTraits::ImplemShared> SharedMutex;
}

#endif /* end of include guard: BASIC_SHARED_Z7A1MWF */
