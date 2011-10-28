#ifndef WINCOMPLETIONWRAPPER_HPP__
#define WINCOMPLETIONWRAPPER_HPP__

#include <tuple>
#include <WinSock2.h>

namespace moost {
namespace stash {
namespace win_ {
	template <class Call, class Buff>
	void _completion_wrapper(DWORD err, DWORD bytes, LPOVERLAPPED over, DWORD)
	{
		std::tuple<Call, Buff> &t = *reinterpret_cast<std::tuple<Call, Buff>*>(over->hEvent);
		std::get<0>(t)(std::get<1>(t), bytes, err);
	}
}
}
}

#endif