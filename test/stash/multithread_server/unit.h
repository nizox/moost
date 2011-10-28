#ifndef UNIT_R5IJ6I3F
#define UNIT_R5IJ6I3F

#include "moost/stash.hpp"
#include "moost/threads.hpp"
#include "client.h"
#include <iostream>
#include <utility>
#include <queue>

using namespace moost::stash;

class unit
{
 public:
  unit(Dispatcher &disp) {
    int fds[2];
    moost::Error err;
    int res = socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    if (res < 0)
      std::cerr << "BITE" << std::endl;
    _out = new IP::TCP::Socket(disp);
    _in = new IP::TCP::Socket(this->_disp);
    _out->assign(fds[0], err);
    _in->assign(fds[1], err);
    _in->set_option(IP::TCP::Socket::NonBlocking(true), err);
    _out->set_option(IP::TCP::Socket::NonBlocking(true), err);
  };

  void run();
  void start(connection_mgr::connection_ptr);
  Dispatcher &dispatcher() {
    return _disp;
  }

  connection_mgr    &manager() {
    return _cm;
  }

  std::pair<IP::TCP::Socket*, IP::TCP::Socket*> get_communication_canal() 
  {
    return std::make_pair(_in, _out);
  }

  void handle_signal();
 private:
  std::vector<connection_mgr::connection_ptr> _queue;
  IP::TCP::Socket *_in;
  IP::TCP::Socket *_out;
  Dispatcher _disp;
  connection_mgr _cm;
  char _buf[16];
  moost::Mutex _mutex;
};

#endif /* end of include guard: UNIT_R5IJ6I3F */
