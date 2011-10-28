#include "moost/threads/win/win_shared.h"

namespace moost {
namespace win_implem {

win_shared::win_shared(): _shared(false)
{
  InitializeSRWLock(&_handle);
}

void  win_shared::read_lock()
{
  AcquireSRWLockShared(&_handle);
  _shared = true;
}

void  win_shared::write_lock()
{
  AcquireSRWLockExclusive(&_handle);
  _shared = false;
}

void  win_shared::try_read_lock()
{
  TryAcquireSRWLockShared(&_handle);
  _shared = true;
}

void  win_shared::try_write_lock()
{
  TryAcquireSRWLockExclusive(&_handle);
  _shared = false;
}

void  win_shared::unlock()
{
  if (_shared)
  {
    ReleaseSRWLockShared(&_handle);
  }
  else
  {
    ReleaseSRWLockExclusive(&_handle);
  }
}

win_shared::~win_shared()
{
}

}
}
