#ifndef BOOOSTDISPATCHER_H__
#define BOOOSTDISPATCHER_H__

#include <boost/asio.hpp>

namespace moost {
namespace stash {

class BoostDispatcher
{
public:
  BoostDispatcher();
  ~BoostDispatcher();

  void run();

private:
  boost::asio::io_service _ioserv;

  template<typename T> friend class BoostSocket;
  template<typename T> friend class BoostSocketAcceptor;
  template<typename T> friend class BoostResolver;
  friend class BoostTimer;
};

} // stash
} // moost

#endif
