#include "moost/threads/posix/posix_cond.h"
#include "moost/threads/posix/posix_mutex.hpp"

namespace		moost {
  namespace		cond {
   	posix_cond::posix_cond() : _cv()
    {
      pthread_cond_t tmp = PTHREAD_COND_INITIALIZER;
      this->_cv = tmp;
      pthread_cond_init(&this->_cv, NULL);
    }

    void		posix_cond::wake()
    {
      pthread_cond_signal(&this->_cv);
    }

    void  posix_cond::wait(mutex::posix_mutex &mutex)
    {
      pthread_cond_wait(&this->_cv, &mutex._handle);
    }

    void  posix_cond::broadcast()
    {
      pthread_cond_broadcast(&this->_cv);
    }

    posix_cond::~posix_cond()
    {
      pthread_cond_destroy(&this->_cv);
    }

  }
}
