#ifndef NONCOPYABLE_8PJT7OIB

#define NONCOPYABLE_8PJT7OIB

namespace moost {

class noncopyable 
{
 protected:
  noncopyable(){};
  virtual ~noncopyable(){};
 private:
  noncopyable(noncopyable const &);
  noncopyable & operator=(noncopyable const &);
};

}

#endif /* end of include guard: NONCOPYABLE_8PJT7OIB */
