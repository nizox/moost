#ifndef BASICRESOLVER_HPP_
#define BASICRESOLVER_HPP_

#include "moost/stash/BasicDispatcher.hpp"
#include "moost/stash/BasicEndpoint.hpp"

namespace moost {
namespace stash {
 
template<typename Protocol, typename Implementation>
class BasicResolverQuery 
{
 public:
  typedef Protocol ProtocolType;

  BasicResolverQuery(std::string const &service)
      : _implem(service) {}
  BasicResolverQuery(ProtocolType const &protocol, std::string const &service)
      : _implem(protocol._implem, service) {}
  BasicResolverQuery(std::string const &hostname, std::string const &service)
      : _implem(hostname, service) {}
  BasicResolverQuery(ProtocolType const &protocol, std::string const &hostname,
             std::string const &service)
      : _implem(protocol._implem, hostname, service) {}
  ~BasicResolverQuery() {}

  std::string get_hostname() const
  {
    return _implem.host_name();
  }

  std::string get_service() const
  {
    return _implem.service_name();
  }

 private:
  Implementation _implem;

  template <typename P, typename I, typename DI> friend class BasicResolver;
};
 
template <typename Protocol, typename Implementation,
          typename DispatcherImplementation>
class BasicResolver
{
 public:
  typedef typename Protocol::Endpoint EndpointType;
  typedef typename Implementation::iterator iterator;
  typedef BasicResolverQuery<Protocol, typename Implementation::Query> Query;

  BasicResolver(BasicDispatcher<DispatcherImplementation> &disp) 
      : _implem(disp._implem) {}
  ~BasicResolver() {}

  iterator resolve(Query const &q, Error &err)
  {
    return _implem.resolve(q._implem, err);
  }

 private:
  Implementation _implem;
};

}
}

#endif
