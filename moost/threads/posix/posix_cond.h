#ifndef				POSIX_COND_H__
# define			POSIX_COND_H__

#include <pthread.h>

namespace			moost {
namespace mutex {
class posix_mutex;
}
  namespace			cond {

    class			posix_cond
    {
    public:
     posix_cond();
     ~posix_cond();
     void			wait(mutex::posix_mutex &);
     void			wake();
     void     broadcast();

    private:
      pthread_cond_t		_cv;
    };

    namespace			CondTraits
    {
      typedef posix_cond	CondImplementation;
    }
  }
}

#endif
