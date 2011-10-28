#ifndef BASICSHAREDLIBRARY_HPP
#define BASICSHAREDLIBRARY_HPP

#include <moost/utils/noncopyable.hpp>

#include <string>

namespace moost {
namespace ic {

template <class Implementation>
class BasicSharedLibrary: moost::noncopyable
{
 public:

  inline BasicSharedLibrary() : _implem()
  {
  }

  inline BasicSharedLibrary(std::string const &path) : _implem(path)
  {
  }

  ~BasicSharedLibrary()
  {
  }

  inline static std::string suffix()
  {
    return Implementation::suffix();
  }

  inline void load(std::string const &path, Error &err)
  {
    _implem.load(path, err);
  }

  inline void unload(Error &err)
  {
    _implem.unload(err);
  }

  inline bool has_symbol(std::string const &symbol)
  {
    return _implem.has_symbol(symbol);
  }

  inline void *get_symbol(std::string const &symbol)
  {
    return _implem.get_symbol(symbol);
  }

 private:
  Implementation _implem;
};

} // !ic
} // !moost

#endif // !BASICSHAREDLIBRARY_HPP
