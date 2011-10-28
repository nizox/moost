#ifndef POSIX_MUTEX_NDRJQ44E

#define POSIX_MUTEX_NDRJQ44E

#include <pthread.h>

namespace moost {
namespace cond {
class posix_cond;
}
  namespace mutex {
    class posix_mutex
    {
    public:
      posix_mutex();
      void lock();
      void try_lock();
      void unlock();
      ~posix_mutex();
    private:
      pthread_mutex_t _handle;
      friend class cond::posix_cond;
    };

    namespace MutexTraits
    {
      typedef posix_mutex MutexImplem;
    }
  }
}

#endif /* end of include guard: POSIX_MUTEX_NDRJQ44E */
