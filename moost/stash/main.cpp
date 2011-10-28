#include "moost/stash.hpp"

int lol()
{
}

int main()
{
  moost::stash::Dispatcher dispatcher;
  moost::stash::IP::TCP::Socket new_socket(dispatcher);
  moost::stash::IP::TCP::Endpoint new_socket_endpoint;
  moost::stash::IP::TCP::Acceptor acceptor(dispatcher, moost::stash::IP::TCP::Endpoint(moost::stash::IP::TCP::v4(), 8989));
  moost::stash::IP::TCP::Resolver resolver(dispatcher);
  moost::stash::IP::TCP::Resolver::Query query("www.google.com", "http");
  moost::stash::IP::TCP::Resolver::iterator it = resolver.resolve(query);
  moost::Error err;

  acceptor.accept(new_socket, new_socket_endpoint, err);
}
