#include <sys/time.h>
#include <list>
#include <vector>
#include <algorithm>
#include "moost/memory.hpp"

using namespace moost;

class Bite
{
 public:
  Bite() : 
      i0(0)
  {
  };
  int i0;
  char bite;
};

class bite :public Bite,  public memory::helper<bite>
{
};

template <typename T>
struct timeval test()
{
    T *b;
    std::vector<T*> l;
    struct timeval start;
    struct timeval stop;

    ::gettimeofday(&start, 0);
    for (unsigned int i = 0; i < 1 << 16;++i)
    {
      b = new T();
      l.push_back(b);
    }
    ::gettimeofday(&stop, 0);
    struct timeval lol;
    timersub(&stop, &start, &lol);
    std::for_each(l.begin(), l.end(), [](T *b) {
                   delete b;
                  });
    return lol;
}

template <typename T>
struct timeval test0()
{
  T* b;
  memory::pool<T> p;
  std::vector<T*> l;
  struct timeval start;
  struct timeval stop;

  ::gettimeofday(&start, 0);
  for (unsigned int i = 0; i < 1 << 16; ++i)
  {
    b = p.get();
    l.push_back(b);
  }
  ::gettimeofday(&stop, 0);
  struct timeval result;
  timersub(&stop, &start, &result);
  std::for_each(l.begin(), l.end(), [&p](T *b) {
                  p.put_back(b);
                });
  return result;
}

int  main(int, const char **)
{
  struct timeval pooled = test<bite>();
  struct timeval notpooled = test<Bite>();
  double tpool, tnotpool;

  tpool = (double)pooled.tv_sec + ((double)pooled.tv_usec / 1000000.f);
  tnotpool = (double)notpooled.tv_sec + ((double)notpooled.tv_usec / 1000000.f);
  std::cout << "speedup is : " << tnotpool / tpool << std::endl; 
  return (0);
}
