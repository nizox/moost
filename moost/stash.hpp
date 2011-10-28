#ifndef STASH_HPP_
#define STASH_HPP_

#include "moost/stash/streambuf.hpp"
#include "moost/stash/BasicDispatcher.hpp"
#include "moost/stash/BasicTimer.hpp"
#include "moost/stash/NetTraits.h"
#include "moost/stash/IP.hpp"
#include "moost/stash/Local.hpp"

namespace moost {
namespace stash {

typedef BasicDispatcher<Traits::ImplemDispatcher> Dispatcher;
typedef BasicTimer<Traits::ImplemTimer, Traits::ImplemDispatcher> Timer;

} // stash
} // moost

#endif
