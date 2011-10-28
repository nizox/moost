#ifndef MUTEXTRAITS_ALCHES0

#define MUTEXTRAITS_ALCHES0

#if defined(UNIX)
#include "moost/threads/posix/posix_mutex.hpp"
#elif defined(WINDOWS)
#include "moost/threads/win/win_mutex.hpp"
#endif

#endif /* end of include guard: MUTEXTRAITS_ALCHES0 */
