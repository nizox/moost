#ifndef POSIX_SHARED_ZO2EWXLV

#define POSIX_SHARED_ZO2EWXLV

#include <pthread.h>

namespace moost {
namespace posix_implem {

class posix_shared
{
 public:
  posix_shared();
  ~posix_shared();
  void read_lock();
  void write_lock();
  void try_read_lock();
  void try_write_lock();
  void unlock();
 private:
  pthread_rwlock_t _handle;
  pthread_rwlockattr_t _attr;
};
}
namespace SharedTraits
{
  typedef posix_implem::posix_shared ImplemShared;
}
}


#endif /* end of include guard: POSIX_SHARED_ZO2EWXLV */
