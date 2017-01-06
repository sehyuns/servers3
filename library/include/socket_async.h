// @file	socket_async.h
//
//

#ifndef _SOCKET_ASYNC_H_
#define _SOCKET_ASYNC_H_

#pragma once

#include "socket.h"

#include <mswsock.h>


namespace lib
{
	class MessageBuffer;
}

namespace lib
{
	enum EOVERLAPPED_TYPE
	{
		  EO_ACCEPT = 0
		, EO_SEND
		, EO_RECEIVE
		, EO_SHUTDOWN
	};

	////////////////////////////////////////////////////////////////////////////////
	// @class	SocketAsync
	//
	// @brief	비동기(Overlapped) 방식을 지원하는 socket API.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class SocketAsync : public Socket
	{
	public:
		SocketAsync();
		~SocketAsync();

	public:
		virtual INT32	initialize(const SOCKET& socket_in = INVALID_SOCKET) override;
		virtual VOID	close() override;

	public:
		INT32			advanced_accept(const SOCKET& socket_in, INT32 key_in);
		INT32			complete_advanced_accept();
		INT32			advanced_connect();

	public:
		INT32			receive_async(MessageBuffer* buf_in);
		INT32			send_async(const MessageBuffer* buf_in);

	public:
		struct AsyncOverlapped : public WSAOVERLAPPED 
		{
			INT32	_type;
			INT32	_key;

			AsyncOverlapped() : _type(EO_ACCEPT), _key(0)	{ memset(this, 0, sizeof(OVERLAPPED)); }
			VOID	set_type(INT32 type_in)			{ _type = type_in; }
			INT32	get_type()						{ return _type; }

		};

		operator LPOVERLAPPED ()					{ return &_recv_overlapped; }

	private:
		AsyncOverlapped		_recv_overlapped;
		AsyncOverlapped		_send_overlapped;

		static LPFN_ACCEPTEX		_accept_func;
		static LPFN_CONNECTEX		_connect_func;
	};
}

#endif