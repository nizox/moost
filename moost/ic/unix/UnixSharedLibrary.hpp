#ifndef UNIXSHAREDLIBRARY_HPP
#define UNIXSHAREDLIBRARY_HPP

#include <dlfcn.h>

#include <moost/utils.hpp>

#include <string>

namespace moost {
namespace ic {

class UnixSharedLibrary: public moost::noncopyable
{
 public:
  inline UnixSharedLibrary() : _handle()
  {
  }

  inline UnixSharedLibrary(std::string const &path) : _handle()
  {
    Error err;

    load(path, err);
  }

  inline ~UnixSharedLibrary()
  {
    Error err;

    unload(err);
  }

  inline static std::string suffix()
  {
    return ".so";
  }

  inline void load(std::string const &path, Error &err)
  {
    if (!_handle)
    {
      _handle = dlopen(path.c_str(), RTLD_LAZY);
      if (_handle == NULL)
        err.assign(dlerror());
    }
  }

  inline void unload(Error &err)
  {
    if (_handle)
    {
      if (dlclose(_handle) != 0)
        err.assign(dlerror());
    }
  }

  inline bool has_symbol(std::string const &symbol)
  {
    return get_symbol(symbol) != NULL;
  }

  inline void *get_symbol(std::string const &symbol)
  {
    return dlsym(_handle, symbol.c_str());
  }

 private:
  void *_handle;
};

} // !ic
} // !moost

#endif // !UNIXSHAREDLIBRARY_HPP
