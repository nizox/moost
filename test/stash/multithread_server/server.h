#ifndef SERVER_RHUZCKTE

#define SERVER_RHUZCKTE

#include "moost/stash.hpp"
#include "client.h"
#include "unit_pool.h"
#include <functional>
#include <vector>
#include <iostream>
#include <utility>

using namespace moost::stash;

class server
{
 public:
  static unsigned int const thread_number = 5;
  server(IP::TCP::Endpoint &local_endpoint) : _accp(_dip, local_endpoint), _units(thread_number, _dip) {
    std::cerr << "Server started on " << local_endpoint << std::endl;
  }
  ~server(){}
  void  run();
 private:
  void  handle_accept(connection_mgr::connection_ptr, moost::Error &err, unit &);

  IP::TCP::Acceptor _accp;
  Dispatcher _dip;
  unit_pool  _units;
  std::vector< std::pair<IP::TCP::Socket*, IP::TCP::Socket*> > _socket_pairs;
};


#endif /* end of include guard: SERVER_RHUZCKTE */
