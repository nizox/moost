#ifndef WINENDPOINT_HPP__
#define WINENDPOINT_HPP__

#include <string>
#include "moost/stash/win/WinIP.hpp"

namespace moost {
namespace stash {
namespace win_ {
	template <typename Protocol>
	struct WinEndpoint
	{
		typedef moost::stash::_WinIP<Protocol> ProtocolType;
		WinEndpoint(ProtocolType const protocol, unsigned short port)
			: _p(protocol), _port(port), _addr("") {}
		WinEndpoint(std::string const &address, unsigned short port)
			: _p(ProtocolType::v4()), _port(port), _addr(address) {}

		ProtocolType const _p;
		unsigned short _port;
		std::string const _addr;
	};
}
}
}

#endif