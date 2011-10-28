#ifndef WINDISPATCHER_H__
#define WINDISPATCHER_H__

#include <WinSock2.h>

namespace moost {
namespace stash {

class WinDispatcher
{
public:
	WinDispatcher();
	~WinDispatcher();
private:
	void run();
	WSADATA		_data;
};
}
}

#endif