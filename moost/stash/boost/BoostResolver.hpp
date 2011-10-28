#ifndef BOOSTRESOLVER_HPP_
#define BOOSTRESOLVER_HPP_

#include <boost/asio.hpp>

#include <vector>
#include <string>
#include <tr1/memory>

#include "moost/utils/Error.hpp"
#include "moost/stash/boost/boost_meta_handle.hpp"
#include "moost/stash/boost/BoostDispatcher.h"

namespace moost {
namespace stash {

template<typename Protocol>
class BoostResolverQuery 
{
  typedef typename meta_handle<Protocol>::TYPE meta_type;
 public:
  typedef Protocol ProtocolType;

  BoostResolverQuery(std::string const &service)
      : _handle(service) {}
  BoostResolverQuery(ProtocolType const &protocol, std::string const &service)
      : _handle(protocol._handle, service) {}
  BoostResolverQuery(std::string const &hostname, std::string const &service)
      : _handle(hostname, service) {}
  BoostResolverQuery(ProtocolType const &protocol, std::string const &hostname,
             std::string const &service)
      : _handle(protocol._handle, hostname, service) {}
  ~BoostResolverQuery() {}

  std::string get_hostname() const
  {
    return _handle.host_name();
  }

  std::string get_service() const
  {
    return _handle.service_name();
  }

 private:
  typename meta_type::resolver::query _handle;

  template <typename P> friend class BoostResolver;
};


template<typename Protocol>
class BoostResolverEntry 
{
  typedef typename meta_handle<Protocol>::TYPE meta_type;
 public:
  typedef Protocol ProtocolType;

  BoostResolverEntry(boost::asio::ip::basic_resolver_entry<meta_type> const &entry)
      : _handle(entry) {}
  ~BoostResolverEntry() {}

  BoostEndpoint<Protocol> get_endpoint() const
  {
    return BoostEndpoint<Protocol>(_handle.endpoint());
  }

  std::string get_hostname() const
  {
    return _handle.host_name();
  }

  std::string get_service() const
  {
    return _handle.service_name();
  }

 private:
  boost::asio::ip::basic_resolver_entry<meta_type> _handle;
};

template<typename Protocol>
class BoostResolverIterator
{
 public:
  typedef typename meta_handle<Protocol>::TYPE meta_type;

  BoostResolverIterator(typename meta_type::resolver::iterator it)
      : _values(new value_type()), _index(0)
  {
    typename meta_type::resolver::iterator ite;

    for (;it != ite; ++it)
      _values->push_back(BoostResolverEntry<Protocol>(*it));
  }
  ~BoostResolverIterator() {}

  BoostResolverEntry<Protocol> const &operator*() const
  {
    return (*_values)[_index];
  }

  BoostResolverEntry<Protocol> const *operator->() const
  {
    return &(*_values)[_index];
  }

 private:
  typedef std::vector<BoostResolverEntry<Protocol> > value_type;
  std::tr1::shared_ptr<value_type> _values;
  size_t _index;
};

template <typename Protocol>
class BoostResolver
{
  typedef typename meta_handle<Protocol>::TYPE meta_type;

 public:
  typedef BoostResolverQuery<Protocol> Query;
  typedef BoostResolverIterator<Protocol> iterator;

  BoostResolver(BoostDispatcher &disp) : _handle(disp._ioserv) {}
  ~BoostResolver() {}

  iterator resolve(Query const &q, Error &err)
  {
    boost::system::error_code berr = boost_error(err);
    iterator wrapper(_handle.resolve(q._handle, berr));

    boost2moost_error(err, berr);
    return wrapper;
  }

 private:
  typename meta_type::resolver _handle;
};

}
}
#endif
