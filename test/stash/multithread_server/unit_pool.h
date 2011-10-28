#ifndef UNIT_POOL_TGHQHN2
#define UNIT_POOL_TGHQHN2

#include "moost/stash.hpp"
#include "moost/threads.hpp"
#include "unit.h"
#include <vector>

using namespace moost::stash;

class unit_pool
{
 public:
  unit_pool(size_t pool_size, Dispatcher &disp) : size(pool_size), current_index(0) {
    for (unsigned int i = 0; i < size; ++i)
      _units.push_back(new unit(disp));
  }
  ~unit_pool() {
  }
  unit &get_unit();
  void run();
  void stop();
 private:
  std::vector<moost::Thread*> threads;
  size_t  size;
  size_t  current_index;
  std::vector<unit*> _units;
};

#endif /* end of include guard: UNIT_POOL_TGHQHN2 */
