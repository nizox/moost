#include "moost/threads.hpp"
#include <iostream>
#include <functional>
#include <ctime>


moost::Mutex raquette;
int i;

int ping()
{
  for (unsigned int b = 0; b < 512; ++b)
  {
    raquette.lock();
    i++;
    raquette.unlock();
  }
}

struct pong
{
    int operator()()
      {
        for (unsigned int b = 0; b < 512; ++b)
        {
          raquette.lock();
          i--;
          raquette.unlock();
        }
      }
};

int        main(int argc, const char *argv[])
{
  int accu = 0;
  for (size_t bite = 0; bite < 64; ++bite)
  {
    pong p;
    moost::Thread t1(p);
    moost::Thread t2(p);
    moost::Thread t3(p);
    moost::Thread t4(p);
    moost::Thread t5(p);
    moost::Thread t0(&ping);
    moost::Thread t01(&ping);
    moost::Thread t02(&ping);
    moost::Thread t03(&ping);
    moost::Thread t04(&ping);

    t0.join();
    t01.join();
    t02.join();
    t03.join();
    t04.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    accu = i;
  }
  if (accu == 0)
    std::cout << "OK" << std::endl;
  else
    std::cout << "KO" << std::endl;
  return (0);
}
