#ifndef MEM_FN_HPP_
#define MEM_FN_HPP_

namespace moost {

//template <typename C>
//struct _fn;
//
//template <typename R, class C, typename ...LIST>
//struct _fn<R (C::*)(LIST...)>
//{
//public:
//  typedef R (C::*mem_ptr)(LIST...);
//  _fn(mem_ptr lol) : _ptr(lol){};
//  R operator ()(C *ptr, LIST... args)
//    {
//      (ptr->*_ptr)(args...);
//    }
//private:
//  mem_ptr _ptr;
//};
//
//template <typename R, class C, typename ...LIST>
//_fn<R (C::*)(LIST...)>   mem_fn(R (C::*ptr)(LIST...))
//{
//  _fn<R (C::*)(LIST...)> t(ptr);
//  return t;
//}

} // moost
#endif
