// @file	message.h
//
//

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#pragma once

#include "shared_defs.h"

namespace shared
{
#pragma pack(push, 1)
	////////////////////////////////////////////////////////////////////////////////
	// @class	Message
	//
	// @brief	네트워크 메세지의 가장 기본이 되는 헤더.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class Header
	{
	public:
		Header(INT32 from_in, BOOL encrypt_in) : _from(from_in), _is_encrypt(encrypt_in) {}
		virtual ~Header() {}

	public:
		INT32			get_from()						{ return _from; }
		VOID			set_from(INT32 from_in)			{ _from = from_in; }

		BOOL			is_encrypt()					{ return _is_encrypt; }

	private:
		INT32			_from;

		BOOL			_is_encrypt;
		
	};


#pragma pack(pop)
}

#endif
