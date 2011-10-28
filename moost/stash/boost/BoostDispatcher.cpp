#include "BoostDispatcher.h"

namespace moost {
namespace stash {

BoostDispatcher::BoostDispatcher() {}
BoostDispatcher::~BoostDispatcher() {}

void BoostDispatcher::run()
{
  _ioserv.run();
}

} // stash
} // moost
