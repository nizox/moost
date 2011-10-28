#include <iostream>
#include "moost/threads.hpp"


moost::Mutex raquette;
moost::Cond condping;
moost::Cond condpong;


int pinf()
{
  raquette.lock();
  for (unsigned int i = 0; i < 10; ++i)
  {
    std::cout << "ping : " << i << std::endl;
    condpong.wake();
    condping.wait(raquette);
  }
  raquette.unlock();
  condpong.wake();
}

struct pong
{
  void  operator()()
  {
    raquette.lock();
    for (unsigned int i = 0; i < 10; ++i)
    {
      std::cout << "pong : " << i << std::endl;
      condping.wake();
      condpong.wait(raquette);
    }
    raquette.unlock();
    condping.wake();
  }
};


int  main(int argc, const char *argv[])
{
  pong p;
  moost::Thread t0(&pinf);
  moost::Thread t1(p);

  t0.join();
  t1.join();
  std::cout << "OK" << std::endl;
  return (0);
}
