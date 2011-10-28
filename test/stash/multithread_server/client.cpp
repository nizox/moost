#include "client.h"

std::string connection::message = "HTTP/1.0 200 OK\r\nServer: SimpleHTTP/0.6 Python/2.7.1\r\nDate: Tue, 08 Mar 2011 10:51:34 GMT\r\nContent-type: text/plain\r\nContent-Length: 20\r\n\r\nbitebitebitebitebite";


connection::~connection()
{
  //std::cerr << "connection closed from " << this->_remote_endpoint << std::endl;
  moost::Error err;
}

void connection::disconnect()
{
  moost::Error err;
  this->_socket.close(err);
  _manager.del(static_cast<connection_mgr::connection_ptr>((this->shared_from_this())));
}

void connection::handle_read(size_t bytes, moost::Error &err)
{
  if (!err)
  {
    char *str = const_cast<char*>(connection::message.c_str());
    Buffer b(str, ::strlen(str));
    _socket.async_send(b, [this](size_t bytes, moost::Error &err) {
                          this->handle_write(bytes, err);
                        });
  }
  else
  {
    //std::cout <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
    this->disconnect();
  }
}

void connection::handle_write(size_t bytes, moost::Error &err)
{
  if (!err)
  {
    this->disconnect();
    /*Buffer b(_data, sizeof(_data));
    _socket.async_receive(b, [this](size_t bytes, moost::Error &err) {
                          this->handle_read(bytes, err);
                        });*/
  }
  else
  {
    //std::cout <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
    this->disconnect();
  }
}

connection_mgr::connection_mgr()
{
}

void connection_mgr::add(connection_ptr ptr)
{
  _list.push_back(ptr);
}

void connection_mgr::del(connection_ptr ptr)
{
  _list.remove(ptr);
}

void connection_mgr::start(connection_ptr new_client)
{
  _list.push_back(new_client);
  //this->set_timeout(new_client);
  this->set_echo(new_client);
}

void connection_mgr::handle_timeout(connection_ptr c, moost::Error &err)
{
    //std::cout <<  __PRETTY_FUNCTION__ << "::["<< err.what() << "]" << std::endl;
    c->disconnect();
}

void connection_mgr::set_echo(connection_ptr c)
{
  Buffer buf(c->_data, sizeof(c->_data));
  c->socket().async_receive(buf, [c](size_t bytes, moost::Error &err) {
                          c->handle_read(bytes, err);
                         });
}

void connection_mgr::set_timeout(connection_ptr c)
{
  // ATTENTION LES TIMEOUT DE MOOST SONT CASSE
  moost::Error err;
  Timer &timer = c->timeout();
  timer.expires_in(10000, err);
  timer.async_wait(std::bind(&connection_mgr::handle_timeout, this,
                             c, std::placeholders::_1));
}
