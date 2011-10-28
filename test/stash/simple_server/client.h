#ifndef CLIENT_S39UNF10

#define CLIENT_S39UNF10

#include "moost/stash.hpp"
#include <memory>
#include <list>

using namespace moost::stash;

class connection_mgr;

class connection : public std::enable_shared_from_this<connection>
{
 public:
  connection(Dispatcher &disp, connection_mgr &cm) : _manager(cm), _socket(disp), _timeout(disp) {}
  ~connection();
  void disconnect();
  void handle_read(size_t , moost::Error&);
  void handle_write(size_t, moost::Error&);
  IP::TCP::Socket &socket()
  {
    return _socket;
  }

  IP::TCP::Endpoint &endpoint()
  {
    return _remote_endpoint;
  }

 private:
  Timer & timeout()
  {
    return _timeout;
  }

  char _data[512];
  connection_mgr &_manager;
  IP::TCP::Socket _socket;
  IP::TCP::Endpoint _remote_endpoint;
  Timer _timeout;
  friend class connection_mgr;
};

class connection_mgr
{
 public:
  typedef std::shared_ptr<connection> connection_ptr;
  connection_mgr();
  void start(connection_ptr);
  void add(connection_ptr);
  void del(connection_ptr);
  void handle_timeout(connection_ptr c, moost::Error &err);
  void set_timeout(connection_ptr c);
  void set_echo(connection_ptr c);
 private:
  std::list<connection_ptr> _list;
};


#endif /* end of include guard: CLIENT_S39UNF10 */
