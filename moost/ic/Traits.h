#ifndef ICTRAITS_H
#define ICTRAITS_H

#if defined(UNIX)
#include "moost/ic/unix/UnixTraits.h"
#elif defined(WINDOWS)
#include "moost/ic/win/WinTraits.h"
#endif

#endif // !ICTRAITS_H
