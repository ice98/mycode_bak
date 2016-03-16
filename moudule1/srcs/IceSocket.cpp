#include "IceSocket.h"

#pragma comment(lib, "Ws2_32.lib")
HERROR Trace0(string _file_path_, DWORD _line_no_)
{
	int err_code = WSAGetLastError();
	string err_info;

	switch(err_code)
	{
	case WSANOTINITIALISED:
		{
			err_info = "A successful WSAStartup call must occur before using this function.";
			break;
		}
	case WSAENETDOWN:
		{
			err_info = "The network subsystem has failed.";
			break;
		}
	case WSAEFAULT:
		{
			err_info = "The buf parameter is not completely contained in a valid part of the \
				user address space.";
			break;
		}
	case WSAENOTCONN:
		{
			err_info = "The socket is not connected.";
			break;
		}
	case WSAEINTR:
		{
			err_info = "The (blocking) call was canceled through WSACancelBlockingCall.";
			break;
		}
	case WSAEINPROGRESS:
		{
			err_info = "A blocking Windows Sockets 1.1 call is in progress, or the \
				service provider is still processing a callback function.";
			break;
		}
	case WSAENETRESET:
		{
			err_info = "The connection has been broken due to the keep-alive activity \
				detecting a failure while the operation was in progress.";
			break;
		}
	case WSAENOTSOCK:
		{
			err_info = "The descriptor is not a socket.";
			break;
		}
	case WSAEOPNOTSUPP:
		{
			err_info = "MSG_OOB was specified, but the socket is not stream-style such \
				as type SOCK_STREAM, OOB data is not supported in the communication \
				domain associated with this socket, or the socket is unidirectional and \
				supports only send operations.";
			break;
		}
	case WSAESHUTDOWN:
		{
			err_info = "The socket has been shut down; it is not possible to receive on \
				a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.";
			break;
		}
	case WSAEINVAL:
		{
			err_info = "The socket has not been bound with bind, or an unknown flag was \
				specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled \
				or (for byte stream sockets only) len was zero or negative.";
			break;
		}
	case WSAEWOULDBLOCK:
		{
			err_info = "The socket is marked as nonblocking and the receive operation \
				would block.";
			break;
		}
	case WSAEMSGSIZE:
		{
			err_info = "The message was too large to fit into the specified buffer and \
				 was truncated.";
			break;
		}
	case WSAETIMEDOUT:
		{
			err_info = "The connection has been dropped because of a network failure \
				or because the peer system failed to respond.";
			break;
		}
	case WSAECONNRESET:
		{
			err_info = "The virtual circuit was reset by the remote side executing a \
				hard or abortive close.";
			break;
		}
		/* Error code specially for calling of WSAStartup() */
	case WSASYSNOTREADY:
		{
			err_info = "the underlying network subsystem is not ready for network communication.";
			break;
		}
	case WSAVERNOTSUPPORTED:
		{
			err_info = "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.";
			break;
		}
	case WSAEPROCLIM:
		{
			err_info = "Limit on the number of tasks supported by the Windows Sockets implementation has been reached.";
			break;
		}
	default:
		{
			err_info = "Unknown socket error.";
		}
	}

	sprintf((char *)err_info.c_str(), "%s, error code %d", err_info, err_code);

	printf("%s\n", err_info.c_str());

	return SUCCESS;
}

HERROR InitSocket(BYTE ver_high, BYTE ver_low)
{
	WSADATA wsa;	
	if (SUCCESS != WSAStartup(MAKEWORD(ver_low, ver_high),  &wsa))
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}

	return SUCCESS;
}

HERROR BindSocket(SOCKET socket, const CHAR* sAddr, WORD wPort)
{	

	SOCKADDR_IN sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = wPort;
	sock_addr.sin_addr.S_un.S_addr = inet_addr(sAddr);

	if (SUCCESS != bind(socket, (SOCKADDR*)&sock_addr, sizeof(sock_addr)))
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}

	return SUCCESS;
}

HERROR ListenSocket(SOCKET socket, int backlog)
{	
	HERROR hRet = SUCCESS;
	if (SUCCESS != listen(socket, backlog))
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}
	return SUCCESS;
}

HERROR AcceptSocket(SOCKET socket, SOCKADDR *addr, int *addr_len, SOCKET *socket_new)
{	
	HERROR ret = accept(socket, addr, addr_len);
	if (ret != INVALID_SOCKET)
	{
		*socket_new = ret;
	}
	else
	{
		socket_new = NULL;
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}

	return SUCCESS;
}

HERROR ConnectSocket(SOCKET socket, const CHAR* sAddr, WORD wPort)
{	
	SOCKADDR_IN sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = wPort;
	sock_addr.sin_addr.S_un.S_addr = inet_addr(sAddr);

	if (SUCCESS != connect(socket,(SOCKADDR*)&sock_addr, sizeof(sock_addr)))
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}

	return SUCCESS;
}


HERROR RecvFromSocket(SOCKET socket, CHAR* buf, int len, int flags, int *rcv_len)
{	
	memset(buf, 0, len);
	HERROR ret = recv(socket, buf, len, flags);

	if (SOCKET_ERROR == ret)
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}
	else
	{
		*rcv_len = ret;
	}

	return SUCCESS;
}

HERROR SendToSocket(SOCKET socket, CHAR* buf, int len, int flags)
{	

	HERROR ret = send(socket, buf, len, flags);

	if (SOCKET_ERROR == ret)
	{
		SOCK_ASSERT0(FALSE);
		return WSAGetLastError();
	}

	return SUCCESS;
}