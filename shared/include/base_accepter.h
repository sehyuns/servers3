// @file	base_accepter.h
//
//

#ifndef _BASE_ACCEPTER_H_
#define _BASE_ACCEPTER_H_

#pragma once

#include "locker.h"
#include "thread.h"

#include "base_receiver.h"


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
	// @class	BaseAccepter
	//
	// @brief	Accept를 기본 동작으로 연결이 완료된 세션으로부터 오는 패킷들을 기본적으로 처리한다.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class BaseAccepter : public lib::Thread<BaseAccepter>, public BaseReceiver
	{
	private:
		typedef		lib::Thread<BaseAccepter>	Thread;

	public:
		BaseAccepter();
		virtual ~BaseAccepter();

	public:
		INT32				initialize(const string& address_in, SHORT port_in, SHORT accept_num=1, SHORT worker_num=1);
		VOID				close();

		INT32				register_acceptors(std::list<Session*>* acceptors);

		INT32				start_accepter();

	public:
		INT32				acceptor_thread();

	protected:
		VOID				disconnect(INT32 key);

		INT32				get_new_client_key();
		VOID				release_client_key(INT32 key_in);

		Session*			get_session(INT32 key);

		virtual VOID		process_incomes(INT32 key_in, UINT32 bytes_in) override;

	protected:
		typedef			std::list<INT32>						TIdList;
		typedef			std::vector<lib::Iocp*>					TIocpList;
		typedef			std::unordered_map<INT32, lib::Iocp*>	TIocpMap;
		typedef			std::unordered_map<INT32, Session*>		TSessionMap;

	private:
		SHORT				_accepter_thread_count;
		SHORT				_worker_thread_count;

		TIocpMap			_iocp_map;
		TIocpList			_iocp_list;
		TSessionMap			_session_map;

		lib::ScopedLocker	_scoped_lock;
		TIdList				_free_session_list;

		lib::SocketAsync*	_listen_socket;

		lib::Iocp*			_accept_iocp;

		lib::SocketAsync*	_acceptor_socket;
	};
}

#endif
