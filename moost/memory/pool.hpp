#ifndef POOL_IW3F003G

#define POOL_IW3F003G

#include <iostream>
#include <algorithm>
#include <list>
#include <vector>

namespace moost {
namespace memory {

template <typename T>
class pool
{
 public:
  pool()
  {
      add_memory();
  }
  
  virtual ~pool()
  {
    std::for_each(blocks.begin(), blocks.end(), [](void *b) {
                    operator delete(b);
                  });
  }

  void destroy()
  {
    std::for_each(blocks.begin(), blocks.end(), [](void *b) {
                    operator delete(b);
                  });
    free.clear();
  }

  void add_memory()
  {
    size_t size = sizeof(T) << 16;
    size_t stop = 1 << 16;
    free.reserve(free.size() + (1 << 16));
#if 0
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    std::cerr << "size of alloced memory : " << size
        << ", number of elements allocated : " << stop << std::endl;
#endif
    T* mem = (T*) operator new (size);
    blocks.push_back(mem);
    for (unsigned int i = 0; i < stop; ++i)
    {
      free.push_back(mem);
      mem++;
    }
  }

  T* get()
  {
    if (free.size() == 0)
      add_memory();
    T* ret = free.back();
    free.pop_back();
    return ret;
  }

  void put_back(T* mem)
  {
    free.push_back(mem);
  }
 private:
  std::vector<T *> free;
  std::list<void*> blocks;
};

template <typename T, template <typename U> class POOL = pool>
class helper
{
 public:
  void* operator new (size_t /*size*/)
  {
    refcnt++;
    return (void*)_pool.get();
  }
  
  void operator delete (void *p)
  {
    refcnt--;
    _pool.put_back((T*)p);
  }
  
  virtual ~helper()
  {
    if (refcnt == 0)
      _pool.destroy();
  }
 private:
  static POOL<T> _pool;
  static int refcnt;
};

template <typename T, template <typename U> class POOL>
int helper<T, POOL>::refcnt = 0;
template <typename T, template <typename U> class POOL>
POOL<T> helper<T, POOL>::_pool;

}
}

#endif /* end of include guard: POOL_IW3F003G */
