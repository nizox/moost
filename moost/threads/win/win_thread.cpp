#include "moost/threads/win/win_thread.h"

namespace moost {
  namespace thread {
    void win_thread::begin(void (*call)(void *), void *ptr)
    {
      this->_id = (HANDLE)_beginthread(call, 0, ptr);
    }

    void win_thread::join()
    {
      WaitForSingleObject(this->_id, INFINITE);
    }

    void win_thread::kill()
    {
      TerminateThread(this->_id, 0);
    }
  }
}
