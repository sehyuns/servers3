// @file	session.h
//
//

#ifndef _SESSION_H_
#define _SESSION_H_

#pragma once

#include "shared_defs.h"

#include "message_buffer.h"
#include "socket_async.h"

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	Session
	//
	// @brief	사용자 접속 정보로서 하나의 연결을 보관하고 활용하는 기본적인 공간이다.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class Session
	{
	public:
		Session(INT32 id_in);
		virtual ~Session();

	public:
		INT32			initialize(const SOCKET& socket_in = INVALID_SOCKET);
		VOID			close();

	public:
		INT32			receive();
		INT32			send(const BYTE* buffer_in, INT32 length_in);
		INT32			send(const lib::MessageBuffer* buffer_in);

	public:
		//virtual INT32	process() = 0;

	public:
		INT32				get_id()						{ return _id; }
		lib::SocketAsync*	get_socket()					{ return &_socket; }

		lib::MessageBuffer*	get_message()					{ return &_message_buffer; }

	private:
		INT32				_id;

		lib::SocketAsync	_socket;
		lib::MessageBuffer	_message_buffer;
	};
}

#endif
