#include "server.h"

void server::run()
{
  moost::Error err;
  connection_mgr::connection_ptr b(new connection(_dip, _cm));
  _accp.async_accept(b->socket(), b->endpoint(), [this, b](moost::Error &err) {
                      this->handle_accept(b, err);
                     });
  _dip.run(err);
  if (err)
    std::cout <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
}

void server::handle_accept(connection_mgr::connection_ptr b, moost::Error &err)
{
  if (!err)
  {
    _cm.start(b);
    // Ajout d'un nouveau client a gerer.
    connection_mgr::connection_ptr n(new connection(_dip, _cm));
    _accp.async_accept(n->socket(), n->endpoint(), [this, n](moost::Error &err) {
                        this->handle_accept(n, err);
                       });
  }
  else
  {
    std::cout <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
  }
}
