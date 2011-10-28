#include <signal.h>
#include <cstring>
#include "moost/utils.hpp"
#include "moost/threads/posix/posix_thread.h"

namespace moost {
  namespace thread {
    void posix_thread::begin(void* (*call)(void*), void *ptr)
    {
      pthread_create(&this->_id, 0, call, ptr);
    }

    void posix_thread::begin(void* (*call)(void*), void *ptr, moost::Error &err)
    {
      int e = pthread_create(&this->_id, 0, call, ptr);
      err.assign(e);
    }

    void posix_thread::join()
    {
      pthread_join(this->_id, 0);
    }

    bool posix_thread::operator==(posix_thread const & other) const
    {
      return pthread_equal(this->_id, other._id);
    }

    void posix_thread::kill()
    {
      pthread_kill(this->_id, SIGINT);
    }
  }
}
