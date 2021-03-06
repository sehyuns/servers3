// @file	base_accepter.cpp
//
//

#include "base_accepter.h"

#include "iocp.h"
#include "socket_async.h"
#include "session.h"


namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseAccepter::BaseAccepter()
		: _listen_socket(nullptr)
		, _accepter_thread_count(0)
		, _worker_thread_count(0)
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseAccepter::~BaseAccepter()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseAccepter::initialize(const string & address_in, SHORT port_in, SHORT accept_num_in, SHORT worker_num_in)
	{
		LOG(DEBUG, "server listening : %s:%d", address_in.c_str(), port_in);

		INT32 result = Thread::initialize();
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		_listen_socket = new lib::SocketAsync();

		result = _listen_socket->initialize();
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		result = _listen_socket->init_server(address_in, port_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}
		_accepter_thread_count = accept_num_in;

//#if !defined(USE_ADVANCED_NETWORK)
		result = Thread::register_thread(string("accepter_thread"), &BaseAccepter::acceptor_thread, accept_num_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		_accepter_thread_count = accept_num_in;
//#else
//		{
//			TIdMap::iterator iter = _id_map.find(string("accepter_thread"));
//			if (iter == _id_map.end())
//			{
//				close();
//				return ERR_THREAD_CREATE_THREAD;
//			}
//
//			for (auto& ii : iter->second)
//			{
//				lib::Iocp* iocp = new lib::Iocp();
//				result = iocp->initialize();
//				if (ERR_SUCCEEDED != result)
//				{
//					close();
//					return ERR_THREAD_CREATE_THREAD;
//				}
//
//				_iocp_map.insert(TIocpMap::value_type(ii, iocp));
//			}
//		}
//#endif

		//result = Thread::register_thread(string("accepter_worker_thread"), &BaseAccepter::worker_thread, worker_num_in);
		//if (ERR_SUCCEEDED != result)
		//{
		//	return result;
		//}

		//{
		//	TIdMap::iterator iter = _id_map.find(string("accepter_worker_thread"));
		//	if (iter == _id_map.end())
		//	{
		//		close();
		//		return ERR_THREAD_CREATE_THREAD;
		//	}

		//	for (auto& ii : iter->second)
		//	{
		//		lib::Iocp* iocp = new lib::Iocp();
		//		result = iocp->initialize();
		//		if (ERR_SUCCEEDED != result)
		//		{
		//			close();
		//			return ERR_THREAD_CREATE_THREAD;
		//		}

		//		_iocp_map.insert(TIocpMap::value_type(ii, iocp));
		//		_iocp_list.push_back(iocp);
		//	}
		//}

		_accept_iocp = new lib::Iocp();

		result = _accept_iocp->initialize();
		if (ERR_SUCCEEDED != result)
		{
			close();
			return result;
		}

		_worker_thread_count = worker_num_in;

		for (INT32 ii = 0; ii < worker_num_in; ++ii)
		{
			lib::Iocp* iocp = new lib::Iocp();

			result = iocp->initialize();
			if (ERR_SUCCEEDED != result)
			{
				close();
				return result;
			}

			_iocp_list.push_back(iocp);
		}

		result = BaseReceiver::initialize(worker_num_in, &_iocp_list);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID	BaseAccepter::close()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseAccepter::register_acceptors(std::list<Session*>* acceptors)
	{
		INT32 sequence_key = 0;

		_accept_iocp = new lib::Iocp();

		_accept_iocp->initialize();

		_accept_iocp->associate((INT32)*_listen_socket, *_listen_socket);

		for (auto& session : *acceptors)
		{
			sequence_key = session->get_id();

#if defined(USE_ADVANCED_NETWORK)
			INT32 result = session->initialize();
			if (ERR_SUCCEEDED != result)
			{
				close();
				return result;
			}

			result = _listen_socket->advanced_accept(*session->get_socket(), sequence_key);
			if (ERR_SUCCEEDED != result)
			{
				close();
				return result;
			}

			//// associate with iocp for accept_thread
			//lib::Iocp* current_iocp = _iocp_list[session->get_id() % _worker_thread_count];
			//if (FALSE == current_iocp->associate(session->get_id(), *session->get_socket()))
			//{
			//	close();
			//	return result;
			//}
#endif

			_session_map.insert(TSessionMap::value_type(sequence_key, session));
			_free_session_list.push_back(sequence_key);
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseAccepter::start_accepter()
	{
		Thread::start_all_threads();
		BaseReceiver::start_all_threads();

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 동기방식의 accept를 사용할때만 활성화되는 thread.
	////////////////////////////////////////////////////////////////////////////////
	INT32	BaseAccepter::acceptor_thread()
	{
		INT32 result = 0;

#if !defined(USE_ADVANCED_NETWORK)
		while (TRUE)
		{
			SOCKET client_socket = _listen_socket->accept();
			if (INVALID_SOCKET == client_socket)
			{
				continue;
			}

			INT32 client_key = get_new_client_key();
			if (0 == client_key)
			{
				LOG(DEBUG, "No more free session for accepting new connection!!");
				continue;
			}

			lib::Iocp* current_iocp = _iocp_list[client_key % _worker_thread_count];

			Session* session = get_session(client_key);
			if (nullptr == session)
			{
				LOG(DEBUG, "No more free session for accepting new connection(%d)!!", client_key);
				continue;
			}

			result = session->initialize(client_socket);
			if (ERR_SUCCEEDED != result)
			{
				continue;
			}

			if (FALSE == current_iocp->associate(client_key, client_socket))
			{
				disconnect(client_key);
				continue;
			}

			LOG(DEBUG, "accepted new connection(%d:%d)!!", session->get_id(), (INT32)client_socket);

			result = session->receive();
			if (ERR_SUCCEEDED != result)
			{
				continue;
			}

			// test
			//if (ERR_SUCCEEDED != session->send((BYTE*)"world", 6))
			//{
			//	LOG(DEBUG, "failed send message!!");
			//}
		}
#else

		while (TRUE)
		{
			INT32 key = 0;
			UINT32 transferred_bytes = 0;
			LPOVERLAPPED overlapped = nullptr;

			result = _accept_iocp->pop_income(key, transferred_bytes, &overlapped);
			if (ERR_SUCCEEDED != result)
			{
				LOG(DEBUG, "disconnected connection(%d)", key);
				continue;
			}

			INT32 session_key = ((lib::SocketAsync::AsyncOverlapped*)overlapped)->_key;
			LOG(DEBUG, "incoming signal(%d)!!", session_key);

			Session* session = get_session(session_key);
			if (nullptr == session)
			{
				LOG(DEBUG, "Failed to find session(%d)", session_key);
				continue;
			}

			session->get_socket()->complete_advanced_accept();

			lib::Iocp* worker_iocp = _iocp_list[session_key % _worker_thread_count];

			worker_iocp->associate(session_key, *session->get_socket());

			session->receive();
		}
#endif

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID BaseAccepter::disconnect(INT32 key_in)
	{
		Session* session = get_session(key_in);
		if (nullptr == session)
		{
			LOG(DEBUG, "Invalid session key(%d)", key_in);
			return;
		}

		session->close();

		release_client_key(key_in);
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseAccepter::get_new_client_key()
	{
		INT32 client_key = 0;
		{
			lib::AutoLocker<lib::ScopedLocker> locker(&_scoped_lock);

			if (_free_session_list.empty())
				return 0;

			client_key = _free_session_list.front();
			_free_session_list.pop_front();
		}

		return client_key;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID	BaseAccepter::release_client_key(INT32 key_in)
	{
		lib::AutoLocker<lib::ScopedLocker> locker(&_scoped_lock);

		_free_session_list.push_back(key_in);
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	Session * BaseAccepter::get_session(INT32 key_in)
	{
		TSessionMap::iterator iter = _session_map.find(key_in);
		if (iter == _session_map.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID BaseAccepter::process_incomes(INT32 key_in, UINT32 bytes_in)
	{
		Session* session = get_session(key_in);
		if (nullptr == session)
		{
			LOG(DEBUG, "Failed to find session(%d)!!", key_in);
			return;
		}

		bytes_in;

		// process packet
		//session->process(key);

		lib::MessageBuffer* message = session->get_message();
		LOG(DEBUG, "received message: %s", message->get_buffer());

		//if (ERR_SUCCEEDED != session->send((BYTE*)"world", 6))
		//{
		//	LOG(DEBUG, "Failed to send message!!(%d)", session->get_id());
		//	disconnect(key_in);
		//	return;
		//}

		// reset session
		INT32 result = session->receive();
		if (ERR_SUCCEEDED != result)
		{
			LOG(DEBUG, "Failed to receive automatically!!(%d)", session->get_id());
			return;
		}
	}


}