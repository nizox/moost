#ifndef WINACCEPTOR_HPP__
#define WINACCEPTOR_HPP__

#include <WinSock2.h>
#include "moost/utils/Error.hpp"

namespace moost {
namespace stash {
	class WinDispatcher;
namespace win_ {

	template <typename Proto>
	class WinAccep
	{
	public:
		typedef typename Proto::Socket Socket;
		typedef typename Proto::Endpoint EndpointType;
		typedef Proto ProtocolType;

		WinAccep(WinDispatcher &di) : _disp(di), _proto(Proto::v4()) {};
		WinAccep(WinDispatcher &di, ProtocolType const &proto) : 
		_disp(di), _proto(proto) {};
		WinAccep(WinDispatcher &di, EndpointType const &ep) : _disp(di), _proto(Proto::v4()), _ep(ep){};

		void open(ProtocolType const &proto, Error &err)
		{
			this->_handle.open(proto, err);
		}

		void close(Error &err)
		{
			this->_handle.close();
		}
	private:
		WinDispatcher &_disp;
		ProtocolType const _proto;
		EndpointType const &_ep;
	};
}
}
}

#endif