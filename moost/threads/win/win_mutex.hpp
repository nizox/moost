#ifndef WIN_MUTEX_HPP__

#define WIN_MUTEX_HPP__

#include <windows.h>

namespace moost {
namespace cond {
	class win_cond;
}
  namespace mutex {
    class win_mutex
    {
    public:
      win_mutex();
      void lock();
      void unlock();
      void try_lock();
      ~win_mutex();

    private:
      CRITICAL_SECTION _handle;

	  friend class moost::cond::win_cond;
    };

    namespace MutexTraits
    {
      typedef win_mutex MutexImplem;
    }
  }
}

#endif
