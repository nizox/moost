#ifndef UNIXRESOLVER_HPP_
#define UNIXRESOLVER_HPP_

#include "moost/utils/Error.hpp"

#include <string>

namespace moost {
namespace stash {

template <typename Protocol>
class UnixQuery
{
};

template <typename Protocol, typename Dispatcher>
class UnixResolver
{
 public:
  typedef void iterator;

  UnixResolver(Dispatcher &disp) : _dispatcher(disp) {}
  ~UnixResolver() {};

  iterator resolve(UnixQuery<Protocol> &q, Error &err)
  {
  }

 private:
  Dispatcher &_dispatcher;
};

}
}
#endif
