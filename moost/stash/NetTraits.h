#ifndef NETTRAITS_H_
#define NETTRAITS_H_

#if defined (BNET)
#include "moost/stash/boost/BoostTraits.h"
#elif defined (UNIX) || defined(WINDOWS)
#include "moost/stash/unix/UnixTraits.h"
//#elif defined (WINDOWS)
//#include "moost/stash/win/WinTraits.h"
#endif

#endif
