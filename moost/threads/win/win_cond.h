#ifndef WIN_COND_H__
#define WIN_COND_H__

#include <windows.h>
#include "moost/threads/win/win_mutex.hpp"

namespace			moost {

  namespace			cond {

    class			win_cond
    {
    public:
     win_cond();
     ~win_cond();
     void			wait(mutex::win_mutex &);
     void			wake();
     void     broadcast();

    private:
      CONDITION_VARIABLE	_cv;
    };

    namespace			CondTraits
    {
      typedef win_cond		CondImplementation;
    }
  }
}

#endif
