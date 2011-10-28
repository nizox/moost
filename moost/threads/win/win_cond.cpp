#include "moost/threads/win/win_cond.h"

namespace		moost {
  namespace		cond {

    win_cond::win_cond()
    {
      InitializeConditionVariable(&this->_cv);
    }

    void		win_cond::wait(mutex::win_mutex &c)
    {
		SleepConditionVariableCS(&this->_cv, &c._handle, INFINITE);
    }

    void  win_cond::wake()
    {
      WakeConditionVariable(&this->_cv);
    }

    void win_cond::broadcast()
    {
      WakeAllConditionVariable(&this->_cv);
    }

    win_cond::~win_cond()
    {
    }
  }
}
