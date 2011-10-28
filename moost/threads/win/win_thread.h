#ifndef WIN_THREAD_U37N6WYC

#define WIN_THREAD_U37N6WYC

#include <windows.h>
#include <process.h>

namespace	moost {
  namespace	thread {

    class	win_thread
    {
    public:
		win_thread() {};
      void	begin(void (*call)(void *), void *ptr);
      void	join();
      void	kill();
      void	cond_wait();

    private:
      HANDLE	_id;
    };

    namespace	ThreadTraits
    {
      typedef win_thread          ThreadImplementation;
      typedef void                ReturnType;
    }
  }
}

#endif /* end of include guard: WIN_THREAD_U37N6WYC */
