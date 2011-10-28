#ifndef POSIX_THREAD_IR1AD71D

#define POSIX_THREAD_IR1AD71D

#include <pthread.h>

namespace	moost {
class Error;
  namespace	thread {

    class	posix_thread
    {
    public:
		posix_thread() : _id() {};
    void	begin(void* (*call)(void*), void *ptr);
    void	begin(void* (*call)(void*), void *ptr, moost::Error &err);
    bool  operator==(posix_thread const &) const;
    void	join();
    void	kill();

    private:
      pthread_t _id;
    };

    namespace	ThreadTraits
    {
      typedef posix_thread        ThreadImplementation;
      typedef void*               ReturnType;
    }
  }
}

#endif /* end of include guard: POSIX_THREAD_IR1AD71D */
