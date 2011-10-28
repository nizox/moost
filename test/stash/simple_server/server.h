#ifndef SERVER_RHUZCKTE

#define SERVER_RHUZCKTE

#include "moost/stash.hpp"
#include "client.h"
#include <functional>
#include <iostream>

using namespace moost::stash;

class server
{
 public:
  server(IP::TCP::Endpoint &local_endpoint) : _accp(_dip, local_endpoint) {
    std::cerr << "Server started on " << local_endpoint << std::endl;
  }
  ~server(){}
  void  run();
 private:
  void  handle_accept(connection_mgr::connection_ptr, moost::Error &err);

  IP::TCP::Acceptor _accp;
  connection_mgr    _cm;
  Dispatcher _dip;
};


#endif /* end of include guard: SERVER_RHUZCKTE */
