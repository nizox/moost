#include "moost/threads/posix/posix_mutex.hpp"

namespace moost {
  namespace mutex {
    posix_mutex::posix_mutex() : _handle()
    { 
      pthread_mutex_t tmp = PTHREAD_MUTEX_INITIALIZER;
      _handle = tmp;
      pthread_mutex_init(&this->_handle, 0);
    }

    posix_mutex::~posix_mutex()
    {
      pthread_mutex_destroy(&this->_handle);
    }

    void posix_mutex::lock()
    {
      pthread_mutex_lock(&this->_handle);
    }

    void posix_mutex::unlock()
    {
      pthread_mutex_unlock(&this->_handle);
    }

    void posix_mutex::try_lock()
    {
      pthread_mutex_trylock(&this->_handle);
    }
  }
}
