#ifndef BOOST_META_HANDLE_HPP_
#define BOOST_META_HANDLE_HPP_

#include <boost/asio.hpp>

namespace moost {
namespace stash {

namespace IP
{
  struct UDP;
  struct TCP;
}

template <typename T>
struct meta_handle
{
  typedef void TYPE;
};

template <>
struct meta_handle<IP::UDP>
{
  typedef class boost::asio::ip::udp TYPE;
};

template <>
struct meta_handle<IP::TCP>
{
  typedef class boost::asio::ip::tcp TYPE;
};

}
}

#endif
