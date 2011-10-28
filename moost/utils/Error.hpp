#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <string.h>
#include <ostream>

namespace moost {

class Error
{
 private:
  typedef void (Error::*safe_bool)();
  void  error_bool(){};
 public:
  inline Error() : _num(0), _msg() {}
  explicit inline Error(int val) : _num(val), _msg()
  {
    this->_msg.assign(strerror(_num));
  }
  explicit inline Error(std::string const &val) : _num(-1), _msg(val) {}
  inline Error(Error const &err) : _num(err._num), _msg(err._msg) {}

  inline Error &operator=(int val)
  {
    this->_num = val;
    this->_msg.assign(strerror(_num));
    return *this;
  }

  inline Error &operator=(std::string const &val)
  {
    this->_num = -1;
    this->_msg = val;
    return *this;
  }

  inline Error &operator=(Error const &err)
  {
    if (this != &err)
    {
      this->_msg = err._msg;
      this->_num = err._num;
    }
    return *this;
  }
  inline ~Error() {}

  inline operator bool() const
  {
    return _num != 0 ? &Error::error_bool : 0;
  }

  inline bool operator==(Error const &err) const 
  {
    return this->_num == err._num;
  }

  inline bool operator!=(Error const &err) const 
  {
    return this->_num != err._num;
  }

  inline bool operator==(int val) const
  {
    return this->_num == val;
  }

  inline bool operator!=(int val) const
  {
    return this->_num != val;
  }

  inline void assign(int val)
  {
    *this = val;
  }

  inline void assign(std::string const &msg)
  {
    *this = msg;
  }

  inline std::string const &what() const
  {
    return _msg;
  }

  inline int code() const
  {
    return _num;
  }

 private:
  int _num;
  std::string _msg;
};
} // moost

#endif /* !ERROR_HPP */
