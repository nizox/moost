#ifndef WIN_SHARED_2C0JCDVV

#define WIN_SHARED_2C0JCDVV

#include <Windows.h>

namespace moost {
namespace win_implem {

class win_shared
{
 public:
  win_shared();
  ~win_shared();

  void read_lock();
  void write_lock();
  void try_read_lock();
  void try_write_lock();
  void unlock();
 private:
  SRWLOCK _handle;
  bool _shared;
};
}
namespace SharedTraits
{
  typedef win_implem::win_shared ImplemShared;
}
}

#endif /* end of include guard: WIN_SHARED_2C0JCDVV */
