// @file	base_connector.h
//
//

#ifndef _BASE_CONNECTOR_H_
#define _BASE_CONNECTOR_H_

#pragma once

#include "thread.h"
#include "base_receiver.h"

#include <unordered_map>

namespace lib
{
	class Iocp;
	class SocketAsync;
}

namespace shared
{
	class Session;
}

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	BaseConnector
	//
	// @brief	지정된 목적지에 접속을 시도하고 접속 완료된 세션으로부터 오늘 패킷을 처리한다.
	//			다중 접속이 가능하며 이를 일괄 처리하도록 한다.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class BaseConnector : public BaseReceiver
	{
	private:
		typedef		lib::Thread<BaseConnector>		Thread;

	public:
		BaseConnector();
		virtual ~BaseConnector();

	public:
		INT32			initialize(SHORT thread_num_in=1);
		VOID			close();

		INT32			start_connector();

		INT32			add_connection(const string& address_in, SHORT port_in, INT32 id_in);

		//INT32			send_message(INT32 );

	public:
		//INT32			worker_thread();

	protected:
		virtual VOID	process_incomes(INT32 key_in, UINT32 bytes_in) override;

	private:
		SHORT			_worker_thread_count;

		lib::Iocp*		_iocp;

		typedef		std::unordered_map<INT32, Session*>		TSessionMap;
		TSessionMap		_session_map;
	};
}

#endif
