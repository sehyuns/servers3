// @file	socket_async.cpp
//
//

#include "socket_async.h"

#include "message_buffer.h"
#include "logger.h"

namespace lib
{
	LPFN_ACCEPTEX	SocketAsync::_accept_func = nullptr;
	LPFN_CONNECTEX	SocketAsync::_connect_func = nullptr;

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	SocketAsync::SocketAsync()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	SocketAsync::~SocketAsync()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32	SocketAsync::initialize(const SOCKET& socket_in)
	{

#if defined(USE_ADVANCED_NETWORK)
		if ((nullptr == _accept_func) || (nullptr == _connect_func))
		{
			SOCKET temp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (INVALID_SOCKET == temp_socket)
			{
				return ERR_SOCKET_CREATE;
			}

			DWORD bytes = 0;
			GUID guid = WSAID_ACCEPTEX;
			if (SOCKET_ERROR == WSAIoctl(temp_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid, sizeof(guid),
				&_accept_func, sizeof(_accept_func),
				&bytes, NULL, NULL))
			{
				recorde_error();
				return ERR_SOCKET_CREATE;
			}

			bytes = 0;
			guid = WSAID_CONNECTEX;
			if (SOCKET_ERROR == WSAIoctl(temp_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid, sizeof(guid),
				&_connect_func, sizeof(_connect_func),
				&bytes, NULL, NULL))
			{
				recorde_error();
				return ERR_SOCKET_CREATE;
			}

			closesocket(temp_socket);
		}
#endif

		if (INVALID_SOCKET == socket_in)
		{
//#if !defined(USE_ADVANCED_NETWORK)
			_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//#else
//			_socket = socket(AF_INET, SOCK_STREAM, 0);
//#endif
			if (INVALID_SOCKET == _socket)
			{
				recorde_error();
				return ERR_SOCKET_CREATE;
			}
		}
		else
		{
			_socket = socket_in;
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID	SocketAsync::close()
	{
		Socket::close();
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SocketAsync::advanced_accept(const SOCKET& socket_in, INT32 key_in)
	{
		DWORD bytes = 0;

		CHAR temp_buffer[(sizeof(SOCKADDR_STORAGE) + 16) * 2] = { 0, };
		AsyncOverlapped overlapped;
		overlapped.set_type(EO_ACCEPT);
		overlapped._key = key_in;
		_recv_overlapped.set_type(EO_ACCEPT);
		_recv_overlapped._key = key_in;

		bytes = 0;
		if (FALSE == _accept_func(_socket, socket_in, &temp_buffer, 0,
									(sizeof(sockaddr_in) + 16), 
									(sizeof(sockaddr_in) + 16), 
									&bytes, &_recv_overlapped))
		{
			recorde_error();
			if (WSA_IO_PENDING != _last_error)
			{
				return ERR_SOCKET_ACCEPT;
			}
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SocketAsync::complete_advanced_accept()
	{
		if (0 != setsockopt(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, NULL, 0))
		{
			recorde_error();
			return ERR_SOCKET_ACCEPT;
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SocketAsync::advanced_connect()
	{
		bind(_socket, (sockaddr*)&_address, sizeof(_address));

		AsyncOverlapped overlapped;
		if (FALSE == _connect_func(_socket, (SOCKADDR*)&_address, sizeof(_address), 
								    NULL, 0, NULL, &overlapped))
		{
			recorde_error();
			if (WSA_IO_PENDING != _last_error)
			{
				return ERR_SOCKET_CONNECT;
			}
		}

		if (0 != setsockopt(_socket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
		{
			recorde_error();
			return ERR_SOCKET_CONNECT;
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SocketAsync::receive_async(MessageBuffer * buf_in)
	{
		memset(&_recv_overlapped, 0, sizeof(OVERLAPPED));
		_recv_overlapped.set_type(EO_RECEIVE);

		DWORD transferred_bytes = 0;
		DWORD flag = 0;
		if (SOCKET_ERROR == WSARecv(_socket, buf_in->get_wsa_buf(), 1, 
									&transferred_bytes, &flag, &_recv_overlapped, NULL))
		{
			if (WSA_IO_PENDING != recorde_error())
				return ERR_SOCKET_RECV;
		}
		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SocketAsync::send_async(const MessageBuffer * buf_in)
	{
		memset(&_send_overlapped, 0, sizeof(OVERLAPPED));
		_send_overlapped.set_type(EO_SEND);

		DWORD transferred_bytes = 0;
		DWORD flag = 0;

		if (SOCKET_ERROR == WSASend(_socket, const_cast<MessageBuffer*>(buf_in)->get_wsa_buf(), 1, 
									&transferred_bytes, flag, &_send_overlapped, NULL))
		{
			if (WSA_IO_PENDING != recorde_error())
				return ERR_SOCKET_SEND;
		}
		return ERR_SUCCEEDED;
	}

}