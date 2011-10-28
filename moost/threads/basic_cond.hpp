#ifndef			BASIC_COND_HPP__
# define		BASIC_COND_HPP__

#include "moost/threads/CondTraits.h"
#include "moost/threads/basic_mutex.hpp"

namespace		moost {
namespace		cond {
template <class Implem>
class		basic_cond
{
 public:
  inline void       init()
  {
    _implem.init();
  }
  template <typename ImplementationMutex>
  void			wait(moost::mutex::basic_mutex<ImplementationMutex> &mutex)
  {
    _implem.wait(mutex._implem);
  }
  void		wake()
  {
    _implem.wake();
  }

  void     cond_broadcast()
  {
    _implem.broadcast();
  }
 private:
  Implem		_implem;
};
}
typedef cond::basic_cond<cond::CondTraits::CondImplementation> Cond;
}

#endif
