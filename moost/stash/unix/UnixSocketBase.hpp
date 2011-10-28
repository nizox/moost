#ifndef UNIXSOCKETBASE_HPP
#define UNIXSOCKETBASE_HPP

#include "moost/utils/Error.hpp"
#include "moost/utils/types.hpp"

namespace moost {
namespace stash {

class UnixSocketBase
{
 public:
  typedef int NativeType;

  STRONG_TYPEDEF(bool, ReuseAddress);
  STRONG_TYPEDEF(bool, NonBlocking);
 
  inline ~UnixSocketBase() {};
};

} // stash
} // moost

#endif /* !UNIXSOCKETBASE_HPP */
