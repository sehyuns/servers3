// @file	iocp.h
//
//

#ifndef _IOCP_H_
#define _IOCP_H_

#pragma once

#include "lib_defs.h"

namespace lib
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	Iocp
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	class Iocp
	{
	public:
		Iocp();
		~Iocp();

	public:
		INT32			initialize();
		VOID			close();

		BOOL			associate(INT32 key_in, const SOCKET& socket_in);

		INT32			pop_income(INT32& key_out, UINT32& bytes_out, LPOVERLAPPED* overlapped_out);

		INT32			push_forcely(INT32 key_in, OVERLAPPED* overlapped_in);

	public:
		INT32			record_error();
		INT32			get_last_error()				{ return _last_error; }

	private:
		HANDLE		_handle;

		INT32		_last_error;
	};
}

#endif