#ifndef BOOSTENDPOINT_HPP
#define BOOSTENDPOINT_HPP

#include <boost/asio.hpp>

#include "moost/stash/boost/boost_meta_handle.hpp"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostEndpoint: public meta_handle<Protocol>::TYPE::endpoint
{

 public:
  typedef Protocol ProtocolType;

  BoostEndpoint() {}

  BoostEndpoint(std::string const &address, unsigned short port)
      : meta_handle<Protocol>::TYPE::endpoint(boost::asio::ip::address::from_string(address), port)
  {
  }

  BoostEndpoint(BoostIP<Protocol> const &protocol, unsigned short port)
      : meta_handle<Protocol>::TYPE::endpoint(protocol._handle, port)
  {
  }

  BoostEndpoint(typename meta_handle<Protocol>::TYPE::endpoint const &ep)
      : meta_handle<Protocol>::TYPE::endpoint(ep)
  {
  }

  void address(std::string const &a)
  {
    address(boost::asio::ip::address::from_string(a));
  }

  std::string address() const
  {
    return meta_handle<Protocol>::TYPE::endpoint::address().to_string();
  }
};

} // stash
} // moost

#endif /* !BOOSTENDPOINT_HPP */
