#include "posix_shared.h"

namespace moost {
namespace posix_implem {

posix_shared::posix_shared() : _handle(), _attr()
{
  pthread_rwlockattr_init(&_attr);
  pthread_rwlockattr_setpshared(&_attr, PTHREAD_PROCESS_SHARED);
  pthread_rwlock_init(&_handle, &_attr);
}

void  posix_shared::read_lock()
{
  pthread_rwlock_rdlock(&_handle);
}

void  posix_shared::write_lock()
{
  pthread_rwlock_wrlock(&_handle);
}

void  posix_shared::try_read_lock()
{
  pthread_rwlock_tryrdlock(&_handle);
}

void  posix_shared::try_write_lock()
{
  pthread_rwlock_trywrlock(&_handle);
}

void  posix_shared::unlock()
{
  pthread_rwlock_unlock(&_handle);
}

posix_shared::~posix_shared()
{
  pthread_rwlockattr_destroy(&_attr);
  pthread_rwlock_destroy(&_handle);
}

}
}
