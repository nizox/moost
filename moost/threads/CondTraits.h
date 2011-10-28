#ifndef		COND_TRAITS_H__
# define	COND_TRAITS_H__

#  if defined(UNIX)
#include "moost/threads/posix/posix_cond.h"
#  elif defined(WIN32)
#include "moost/threads/win/win_cond.h"
#  endif

#endif
