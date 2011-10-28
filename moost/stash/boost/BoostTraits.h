#ifndef BOOSTTRAITS_H_
#define BOOSTTRAITS_H_

#include "moost/stash/boost/BoostIP.hpp"
#include "moost/stash/boost/BoostEndpoint.hpp"
#include "moost/stash/boost/BoostSocketDatagram.hpp"
#include "moost/stash/boost/BoostSocketStream.hpp"
#include "moost/stash/boost/BoostSocketAcceptor.hpp"
#include "moost/stash/boost/BoostDispatcher.h"
#include "moost/stash/boost/BoostTimer.h"
#include "moost/stash/boost/BoostResolver.hpp"

namespace moost {
namespace stash {
namespace Traits {
     template <typename T>
       struct ImplemIP
         {
           typedef BoostIP<T> TYPE;
         };
     template <typename T>
       struct ImplemEndpoint
         {
           typedef BoostEndpoint<T> TYPE;
         };
     template <typename T>
       struct ImplemSocketStream
         {
           typedef BoostSocketStream<T> TYPE;
         };
     template <typename T>
       struct ImplemSocketDatagram
         {
           typedef BoostSocketDatagram<T> TYPE;
         };
     template <typename T>
       struct ImplemSocketAcceptor
         {
           typedef BoostSocketAcceptor<T> TYPE;
         };
     template <typename T>
       struct ImplemResolver
         {
           typedef BoostResolver<T> TYPE;
         };
     typedef BoostDispatcher ImplemDispatcher;
     typedef BoostTimer ImplemTimer;
}
}
}

#endif
