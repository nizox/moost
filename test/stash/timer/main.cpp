#include "moost/stash.hpp"

#include <cassert>
#include <ctime>

int test1()
{
  moost::Error err;
  moost::stash::Dispatcher dispatcher;
  int now = time(NULL);
  bool ack = false;
  moost::stash::Timer timer(dispatcher, now + 2);

  std::cout << "[" << now << "] waiting for 2 seconds" << std::endl;

  timer.async_wait([&ack](moost::Error &) {
      int now = time(NULL);

      ack = true;
    std::cout << "[" << now << "] done" << std::endl;
  });

  dispatcher.run(err);

  if (!ack || err)
    return 1;
  return 0;
}

int test2()
{
  moost::Error err;
  moost::stash::Dispatcher dispatcher;
  bool ack = false;
  moost::stash::Timer timer(dispatcher, time(NULL) + 10);

  std::cout << "testing cancel" << std::endl;
  timer.async_wait([&ack](moost::Error &) {
      ack = true;
  });
  timer.cancel(err);

  dispatcher.run(err);

  if (ack || err)
    return 1;
  return 0;
}

int main()
{
  assert(test1() == 0);
  assert(test2() == 0);
}
