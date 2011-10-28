#ifndef UNIXTRAITS_H_
#define UNIXTRAITS_H_

#include "moost/stash/unix/UnixProtocol.hpp"
#include "moost/stash/unix/UnixEndpoint.hpp"
#include "moost/stash/unix/UnixSocketDatagram.hpp"
#include "moost/stash/unix/UnixSocketStream.hpp"
#include "moost/stash/unix/UnixSocketAcceptor.hpp"
#include "moost/stash/unix/UnixDispatcher.hpp"
#include "moost/stash/unix/UnixTimer.hpp"
#include "moost/stash/unix/UnixResolver.hpp"
#include "moost/stash/unix/UnixSocketPair.hpp"

#ifdef LINUX
#include "moost/stash/unix/LinuxReactor.hpp"
#elif FREEBSD
#include "moost/stash/unix/KQueueReactor.hpp"
#else
#include "moost/stash/unix/SelectReactor.hpp"
#endif

namespace moost {
namespace stash {
namespace Traits {
#ifdef LINUX
     typedef LinuxReactor ImplemReactor;
#elif FREEBSD
     typedef KQueueReactor ImplemReactor;
#else
     typedef SelectReactor ImplemReactor;
#endif

     typedef UnixDispatcher<ImplemReactor> ImplemDispatcher;

     template <typename T>
       struct ImplemIP
         {
           typedef UnixIP<T> TYPE;
         };
     template <typename T>
       struct ImplemLocal
         {
           typedef UnixLocal<T> TYPE;
         };

     template <typename T>
       struct ImplemEndpoint
         {
           typedef UnixEndpoint<T> TYPE;
         };
     template <typename T>
       struct ImplemSocketStream
         {
           typedef UnixSocketStream<T, ImplemDispatcher> TYPE;
         };
     template <typename T>
       struct ImplemSocketDatagram
         {
           typedef UnixSocketDatagram<T, ImplemDispatcher> TYPE;
         };
     template <typename T>
       struct ImplemSocketAcceptor
         {
           typedef UnixSocketAcceptor<T, ImplemDispatcher> TYPE;
         };
     template <typename T>
       struct ImplemResolver
         {
           typedef UnixResolver<T, ImplemDispatcher> TYPE;
         };
     template <typename T>
       struct ImplemSocketPair
         {
           typedef UnixSocketPair<T, ImplemDispatcher> TYPE;
         };

     typedef UnixTimer<ImplemDispatcher> ImplemTimer;
}
}
}

#endif
