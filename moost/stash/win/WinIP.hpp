#ifndef WINIP_BLABLA_HPP__
#define WINIP_BLABLA_HPP__

#include <WinSock2.h>

namespace moost {
namespace stash {

template <typename Protocol>
class _WinIP;

namespace IP{
struct UDP;
struct TCP;
}

template <>
class _WinIP<IP::TCP>
{
public:
	static _WinIP<IP::TCP> v4()
	{
		_WinIP<IP::TCP> tmp(AF_INET);
		return tmp;
	}

	static _WinIP<IP::TCP> v6()
	{
		_WinIP<IP::TCP> tmp(AF_INET6);
		return tmp;
	}

	inline int domain() const
	{
		return _domain;
	}

	inline int type() const
	{
		return SOCK_STREAM;
	}

	inline int protocol() const
	{
		return IPPROTO_TCP;
	}
private:
	template <typename P>
	_WinIP(_WinIP<P> const & t){
		if (this != &t)
			_domain = t.domain();
	};
	_WinIP(int d) : _domain(d)  {};

	int _domain;
	template<typename T> friend class WinEndpoint;
	template<typename T> friend class WinSocket;
	template<typename T> friend class WinSocketAcceptor;
};

template <>
class _WinIP<IP::UDP>
{
public:
	static _WinIP<IP::UDP> v4()
	{
		_WinIP<IP::UDP> tmp(AF_INET);
		return tmp;
	}

	static _WinIP<IP::UDP> v6()
	{
		_WinIP<IP::UDP> tmp(AF_INET6);
		return tmp;
	}

	inline int domain()
	{
		return this->_domain;
	}

	inline int type()
	{
		return SOCK_DGRAM;
	}

	inline int protocol()
	{
		return IPPROTO_UDP;
	}
private:
	_WinIP(int t): _domain(t){};
	int _domain;
};
}
}

#endif