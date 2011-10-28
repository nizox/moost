#ifndef IC_HPP
#define IC_HPP

#include "moost/ic/Traits.h"
#include "moost/ic/BasicSharedLibrary.hpp"

namespace moost {
namespace ic {

typedef BasicSharedLibrary<ImplemSharedLibrary> SharedLibrary;

} // !ic
} // !moost

#endif // !IC_HPP
