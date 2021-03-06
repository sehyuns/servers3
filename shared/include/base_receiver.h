// @file	base_receiver.h
//
//

#ifndef _BASE_RECEIVER_H_
#define _BASE_RECEIVER_H_

#pragma once

#include "thread.h"

#include <vector>
#include <unordered_map>

namespace lib
{
	class Iocp;
}

namespace shared
{
	class Session;
}

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	BaseReceiver
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	class BaseReceiver : public lib::Thread<BaseReceiver>
	{
	private:
		typedef		lib::Thread<BaseReceiver>		Thread;

	public:
		BaseReceiver();
		virtual ~BaseReceiver();

	public:
		INT32			initialize(SHORT thread_num_in, std::vector<lib::Iocp*>* iocp_list_in=nullptr);
		VOID			close();

		INT32			register_client(INT32 key_in, Session* session_in);

	public:
		INT32			worker_thread();

	protected:
		virtual VOID	process_incomes(INT32 key_in, UINT32 bytes_in) = 0;

	private:
		typedef			std::unordered_map<INT32, lib::Iocp*>	TIocpMap;
		TIocpMap		_iocp_map;

		typedef			std::vector<lib::Iocp*>					TIocpList;
		TIocpList		_iocp_list;
	};
}

#endif
