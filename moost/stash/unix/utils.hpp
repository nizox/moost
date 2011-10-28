// Header
// (|€~

#ifndef _UTILS_HPP
# define _UTILS_HPP

#if defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


#include <cstdint>
#include <time.h>

#pragma comment(lib, "Ws2_32.lib")

typedef int ssize_t;
#define socklen_t int

#define EAGAIN WSAEWOULDBLOCK
#define ESHUTDOWN 108
#define errno WSAGetLastError()


// Time related defines/functions

#ifndef timerisset
# define timerisset(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)
#endif
#ifndef timerclear
# define timerclear(tvp)	((tvp)->tv_sec = (tvp)->tv_usec = 0)
#endif
#ifndef timercmp
# define timercmp(a, b, CMP) 						      \
  (((a)->tv_sec == (b)->tv_sec) ? 					      \
   ((a)->tv_usec CMP (b)->tv_usec) : 					      \
   ((a)->tv_sec CMP (b)->tv_sec))
#endif
#ifndef timeradd
# define timeradd(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;			      \
    if ((result)->tv_usec >= 1000000)					      \
      {									      \
	++(result)->tv_sec;						      \
	(result)->tv_usec -= 1000000;					      \
      }									      \
  } while (0)
#endif
#ifndef timersub
# define timersub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_usec += 1000000;					      \
    }									      \
  } while (0)
#endif

inline int usleep(long t)
{
  Sleep(t / 1000);
  return 0;
}

# if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
# else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
# endif
 
struct timezone
{
  int  tz_minuteswest;
  int  tz_dsttime;
};
 
inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
 
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    tmpres /= 10;
 
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
 
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
  
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return 0;
}

// fd related functions

inline int close(int fd)
{
  return closesocket(fd);
}

inline int inet_aton(char const *str, struct in_addr *addr)
{
  return ::InetPton(AF_INET, str, addr);
}

#else // WIN32

#include <sys/socket.h>
#include <arpa/inet.h>

#endif

#endif
