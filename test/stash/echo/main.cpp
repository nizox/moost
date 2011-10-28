#include "moost/stash.hpp"

#include <iostream>

#include <tr1/functional>
#include <tr1/memory>

using namespace moost::stash;

class EchoServer
{
 public:
  typedef std::tr1::shared_ptr<IP::TCP::Socket> sptr;
  EchoServer(IP::TCP::Endpoint local_endpoint) : 
      _accp(_disp, local_endpoint), b(new char[512], 512)
  {
    moost::Error err;
    _accp.set_option(IP::TCP::Acceptor::NonBlocking(true), err);
  };


  void  handle_send(sptr s, size_t bytes, moost::Error &err)
  {
    if (!err)
    {
      s->async_receive(b, std::tr1::bind(&EchoServer::handle_receive, this, s, 
                                         std::tr1::placeholders::_1 ,
                                         std::tr1::placeholders::_2));
    }
    else
      std::cerr << err.what() << std::endl;
  }

  void  handle_receive(sptr s, size_t bytes, moost::Error &err)
  {
    if (!err)
    {
      Buffer c(b.data<char*>(), bytes);
      s->async_send(c, std::tr1::bind(&EchoServer::handle_send, this, s,
                                      std::tr1::placeholders::_1,
                                      std::tr1::placeholders::_2));
    }
    else
      std::cerr << err.what() << std::endl;
  }

  void  handle_accp(sptr s, moost::Error &err)
  {
    if (!err)
    {
      s->set_option(IP::TCP::Socket::NonBlocking(true), err);
      std::cout << s->local_endpoint(err) << std::endl;
      std::cout << s->remote_endpoint(err) << std::endl;
      s->async_receive(b, std::tr1::bind(&EchoServer::handle_receive, this, s, 
                                        std::tr1::placeholders::_1 ,
                                        std::tr1::placeholders::_2));
    sptr pending(new IP::TCP::Socket(_disp));
    _accp.async_accept(*pending, pending_endpoint, std::tr1::bind(&EchoServer::handle_accp, this, pending,
                                                  std::tr1::placeholders::_1));
    }
    else
      std::cerr << err.what() << std::endl;
  }

  void run()
  {
    moost::Error err;

    sptr pending(new IP::TCP::Socket(_disp));
    _accp.async_accept(*pending, pending_endpoint, std::tr1::bind(&EchoServer::handle_accp, this, pending,
                                                  std::tr1::placeholders::_1));
    _disp.run(err);
    if (err)
      std::cerr << err.what() << std::endl;
  }
 private:
  Dispatcher _disp;
  IP::TCP::Acceptor _accp;
  Buffer b;
  IP::TCP::Endpoint pending_endpoint;
};


int  main(int argc, const char *argv[])
{
  IP::TCP::Endpoint endpoint(IP::TCP::v4(), 8080);

  EchoServer s(endpoint);

  s.run();
  return (0);
}
