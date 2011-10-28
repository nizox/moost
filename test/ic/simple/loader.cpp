#include <iostream>
#include <moost/ic.hpp>

int main()
{
  moost::Error err;
  moost::ic::SharedLibrary sl1;
  moost::ic::SharedLibrary sl2;
  int (*test)() = NULL;

  sl1.load("./libsimple" + moost::ic::SharedLibrary::suffix(), err);
  if (err)
    goto error;
  sl2.load("./libpwet" + moost::ic::SharedLibrary::suffix(), err);
  if (err)
    goto error;

  if (!sl1.has_symbol("test"))
    std::cout << "has no test symbol" << std::endl;

  if (!sl2.has_symbol("test"))
    std::cout << "has no test symbol" << std::endl;

  test = reinterpret_cast<int (*)()>(sl1.get_symbol("test"));
  test();
  test = reinterpret_cast<int (*)()>(sl2.get_symbol("test"));
  test();

  sl1.unload(err);
  if (err)
    goto error;
  sl2.unload(err);
  if (err)
    goto error;

  return 0;
error:
  std::cout << err.what() << std::endl;
  return 1;
}
