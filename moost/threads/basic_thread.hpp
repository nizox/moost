#ifndef BASIC_THREAD_HYZ878KM

#define BASIC_THREAD_HYZ878KM

#include <tuple>
#include <functional>
#include "moost/threads/ThreadTraits.h"
#include "moost/utils.hpp"

namespace moost {
namespace thread {
namespace ti_ {

template <class Call>
thread::ThreadTraits::ReturnType _call(void *value)
{
  std::tuple<Call> *t = reinterpret_cast<std::tuple<Call>*>(value);
  std::get<0>(*t)();
  return THREAD_RETURN;
}

}

template <class Implem>
class basic_thread
{
 public:
  template <typename Call>
  basic_thread(Call c)
  {
    std::tuple<Call> *t = new std::tuple<Call>(c);
    _implem.begin(ti_::_call<Call>, t);
  }

  template <typename Call>
  basic_thread(Call c, moost::Error &err)
  {
    std::tuple<Call> *t = new std::tuple<Call>(c);
    _implem.begin(ti_::_call<Call>, t, err);
  }

  inline bool operator==(basic_thread<Implem> const & other)
  {
    return this->_implem == other._implem;
  }

  inline void join()
  {
    _implem.join();
  }
 private:
  Implem _implem;
};

}
typedef thread::basic_thread<thread::ThreadTraits::ThreadImplementation> Thread;
}
#endif /* end of include guard: BASIC_THREAD_HYZ878KM */
