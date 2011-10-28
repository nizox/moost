#include "moost/threads/win/win_mutex.hpp"

namespace moost {
  namespace mutex {
    win_mutex::win_mutex()
    {
      InitializeCriticalSection(&this->_handle);
    }

    win_mutex::~win_mutex()
    {
      DeleteCriticalSection(&this->_handle);
    }

    void win_mutex::lock()
    {
      EnterCriticalSection(&this->_handle);
    }

    void win_mutex::unlock()
    {
      LeaveCriticalSection(&this->_handle);
    }

    void  win_mutex::try_lock()
    {
      TryEnterCriticalSection(&this->_handle);
    }
  }
}
