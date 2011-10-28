#ifndef WINSOCKETSTREAM_HPP__
#define WINSOCKETSTREAM_HPP__

#include "moost/stash/win/WinSocket.hpp"

namespace moost {
namespace stash {
namespace win_ {
	template <typename T>
	class WinSocketStream : public WinSocket<T>
	{
	public:
		typedef _WinIP<T> ProtocolType;
		typedef typename WinEndpoint<T> EndpointType;

		WinSocketStream(WinDispatcher &d):
			WinSocket<T>(d) {};
		WinSocketStream(WinDispatcher &d, ProtocolType const &pr):
			WinSocket<T>(d, pr) {};
		WinSocketStream(WinDispatcher &d, EndpointType const &ep):
			WinSocket<T>(d, ep){};
/*
		void open(ProtocolType const &proto, Error &err)
		{
			this->_handle = WSASocket(proto.domain(), proto.type(), proto.protocol(),
				0, 0, WSA_FLAG_OVERLAPPED);
		}

		template <typename Rec>
		void async_receive(Buffer b, Rec rcb)
		{
			WSABUFF wrap;
			WSAOVERLAPPED over;
			SecureZeroMemory((PVOID)&over, sizeof(WSAOVERLAPPED));
			wrap.buf = b.data<char *>();
			wrap.len = b.size();
			std::tuple<Rec, Buffer> *t = new std::tuple<Rec, Buffer>(rcb, b);
			over.hevent = t;
			WSARecv(this->_handle, &wrap, 1, 0, 0, &over, _completion_wrapper<Rec, Buffer>);
		}

		template <typename Send>
		void async_send(Buffer b, Send scb)
		{
			WSABUFF wrap;
			WSAOVERLAPPED over;
			SecureZeroMemory((PVOID)&over, sizeof(WSAOVERLAPPED));
			wrap.buf = b.data<char *>();
			wrap.len = b.size();
			std::tuple<Send> *t = new std::tuple<Send>(scb);
			over.hevent = t;
			WSASend(this->_handle, &wrap, 1, 0, 0, &over, _completion_wrapper<Send>);
		}

		void connect(EndpointType const &end, Error &)
		{
			struct timeval time = {5000, 0};
			std::stringstream ss;
			std::string str;
			ss << end._port;
			str = ss.str();
			if (!WSAConnectByName(this->_handle, end._addr.c_str(), str.c_str(), 0, 0, 0, 0, &time, 0))
			{
				int er = WSAGetLastError();
				std::cout << "ERROR" << std::endl;
			}
		}
		template <typename Call>
		void async_connect(EndpointType const &end, Call c)
		{
			Error err;
			this->connect(Endpoint, err);
			c(err);
		}
		void close(Error &err)
		{
			closesocket(this->_handle);
		}

	private:
		SOCKET _handle;
		WinDispatcher &_disp;
		ProtocolType _proto;
		EndpointType const _ep;
*/
	};
}
}
}

#endif