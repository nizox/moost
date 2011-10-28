#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>

void server::run()
{
  moost::Error err;

  _accp.set_option(IP::TCP::Acceptor::NonBlocking(true), err);

  unit &u = _units.get_unit();
  connection *b = new connection(u.dispatcher(), u.manager());
  _accp.async_accept(b->socket(), b->endpoint(), [this, b, &u](moost::Error &err) {
                      connection_mgr::connection_ptr ptr(b);
                      this->handle_accept(ptr, err, u);
                     });
  _units.run();
  _dip.run(err);
  if (err)
    std::cerr <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
  _units.stop();
}

void server::handle_accept(connection_mgr::connection_ptr b, moost::Error &err, unit &u)
{
  if (!err)
  {
    b->socket().set_option(IP::TCP::Socket::NonBlocking(true), err);
    u.start(b);
    // Ajout d'un nouveau client a gerer.
    unit &nu = _units.get_unit();
    connection_mgr::connection_ptr n(new connection(nu.dispatcher(), nu.manager()));
    _accp.async_accept(n->socket(), n->endpoint(), [this, n, &nu](moost::Error &err) {
                        this->handle_accept(n, err, nu);
                       });
  }
  else
  {
    std::cerr <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
  }
}
