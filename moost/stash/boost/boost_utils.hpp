#ifndef BOOST_UTILS_HPP
#define BOOST_UTILS_HPP

#include "moost/stash/Buffer.hpp"
#include "moost/utils/Error.hpp"

#include <boost/asio.hpp>

namespace moost {

inline void boost2moost_error(Error &err, boost::system::error_code const &berr)
{
  err.assign(berr.value());
}

inline Error moost_error(boost::system::error_code const &berr)
{
  Error err;

  boost2moost_error(err, berr);
  return err;
}

inline boost::system::error_code boost_error(Error const &err)
{
  return boost::system::error_code(err.code(), boost::system::system_category());
}

namespace stash {

inline boost::asio::mutable_buffers_1 boost_buffer(Buffer const &buf)
{
  return boost::asio::buffer(buf.data<void *>(), buf.size());
}


}

} // moost

#endif /* !BOOST_UTILS_HPP */
