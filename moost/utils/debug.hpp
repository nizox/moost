#ifndef DEBUG_YA7CTZCC

#define DEBUG_YA7CTZCC

#include "singleton.hpp"
#include <iostream>

namespace moost {
namespace debug_impl_ {
class debug
{
 public:
  template <typename T>
  debug & operator<<(T const &item)
  {
#if not defined(MOOST_NDEBUG)
    std::cerr << item;
#endif
    return *this;
  }

  debug & operator<<(std::ios_base& (*fn)(std::ios_base&))
  {
#if not defined(MOOST_NDEBUG)
    std::cerr << std::endl; // some crazy crap for doing it whit the FUCKING std::endl TEMPLATE FUNCTION !!??
#endif
    return *this;
  }
  debug & operator<<(std::ostream& (*fn)(std::ostream&))
  {
#if not defined(MOOST_NDEBUG)
    std::cerr << std::endl; // some crazy crap for doing it whit the FUCKING std::endl TEMPLATE FUNCTION !!??
#endif
    return *this;
  }
};

}
debug_impl_::debug debug;
}

#endif /* end of include guard: DEBUG_YA7CTZCC */
