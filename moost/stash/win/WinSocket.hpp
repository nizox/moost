#ifndef WINSOCKET_HPP__
#define WINSOCKET_HPP__

#include <WinSock2.h>
#include <string>
#include <sstream>
#include <tuple>
#include <iostream>
#include <stdio.h>
#include <WS2tcpip.h>
#include "moost/stash/Buffer.hpp"
#include "moost/stash/win/WinIP.hpp"
#include "moost/stash/win/win_completion_wrapper.hpp"

namespace moost {
namespace stash {
namespace win_ {
	template <typename T>
	class WinSocket
	{
	public:
		typedef _WinIP<T> ProtocolType;
		typedef WinEndpoint<T> EndpointType;

		WinSocket(WinDispatcher &d):
		_disp(d), _proto(ProtocolType::v4()) {};
		WinSocket(WinDispatcher &d, ProtocolType const &pr):
		_disp(d), _proto(pr){};
		WinSocket(WinDispatcher &d, EndpointType const &):
		_disp(d), _proto(ProtocolType::v4()){};

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

		void bind(EndpointType const &end, Error &err)
		{
			std::stringstream ss;
			std::string sport;
			ADDRINFOA hints;
			PADDRINFOA list;
			ss << end._port;
			sport = ss.str();
			SecureZeroMemory((PVOID)&hints, sizeof(hints));
			hints.ai_flags = (AI_PASSIVE | AI_CANONNAME);
			hints.ai_family = end._p.domain() != this->_proto.domain() ? AI_ALL : this->_proto.domain();
			hints.ai_socktype = end._p.type();
			hints.ai_protocol = end._p.protocol();
			getaddrinfo(end._addr.c_str(), sport.c_str(), &hints, &list);
			int i = 1;
			for (PADDRINFOA ptr = list; ptr != NULL; ptr = ptr->ai_next)
			{
				        printf("getaddrinfo response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
        printf("\tFamily: ");
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
        printf("\tSocket type: ");
        switch (ptr->ai_socktype) {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol) {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
			}
		}
	private:
		SOCKET _handle;
		WinDispatcher &_disp;
		ProtocolType _proto;
		//EndpointType const _ep;
	};
}
}
}

#endif