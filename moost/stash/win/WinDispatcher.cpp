#include "moost/stash/unix/utils.hpp"
#include "moost/stash/win/WinDispatcher.h"

namespace moost {
namespace stash {

	WinDispatcher::WinDispatcher()
	{
		::WSAStartup(MAKEWORD(2, 2), &this->_data);
	}
	WinDispatcher::~WinDispatcher()
	{
		::WSACleanup();
	}
}
}