#ifndef WINTRAITS_H__
#define WINTRAITS_H__

#include "moost/stash/win/WinDispatcher.h"
#include "moost/stash/win/WinIP.hpp"
#include "moost/stash/win/WinEndpoint.hpp"
#include "moost/stash/win/WinAcceptor.hpp"
#include "moost/stash/win/WinSocketStream.hpp"

namespace moost {
namespace stash {

namespace Traits {
	template <typename T>
	struct ImplemIP
	{
		typedef _WinIP<T> TYPE;
	};
	template <typename T>
	struct ImplemEndpoint
	{
		typedef win_::WinEndpoint<T> TYPE;
	};

	template <typename T>
	struct ImplemSocketAcceptor
	{
		typedef win_::WinAccep<T> TYPE;
	};

	template <typename T>
	struct ImplemSocketStream
	{
		typedef win_::WinSocketStream<T> TYPE;
	};

	template <typename T>
	struct ImplemSocketDatagram
	{
		typedef win_::WinSocketDatagram<T> TYPE;
	};

	typedef WinDispatcher ImplemDispatcher;
}

}
}

#endif