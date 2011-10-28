#include "moost/stash.hpp"
#include "server.h"

using namespace moost::stash;

int  main(int argc, const char *argv[])
{
  IP::TCP::Endpoint end(IP::TCP::v4(), 8080);
  server s(end);
  s.run();
  return (0);
}
