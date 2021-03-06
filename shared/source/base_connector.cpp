// @file	base_connector.cpp
//
//

#include "base_connector.h"

#include "iocp.h"
#include "socket_async.h"
#include "session.h"

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseConnector::BaseConnector()
		: _iocp(nullptr)
		, _worker_thread_count(0)
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseConnector::~BaseConnector()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32	BaseConnector::initialize(SHORT thread_num_in)
	{
		INT32 result = BaseReceiver::initialize(thread_num_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		//_iocp = new lib::Iocp();

		//INT32 result = _iocp->initialize();
		//if (ERR_SUCCEEDED != result)
		//{
		//	return result;
		//}

		//result = Thread::register_thread(string("connector_worker_thread"), &BaseConnector::worker_thread, thread_num_in);
		//if (ERR_SUCCEEDED != result)
		//{
		//	return result;
		//}

		//_worker_thread_count = thread_num_in;

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID	BaseConnector::close()
	{

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseConnector::start_connector()
	{
		BaseReceiver::start_all_threads();

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32	BaseConnector::add_connection(const string& address_in, SHORT port_in, INT32 id_in)
	{
		Session* session = new Session(id_in);

		INT32 result = session->initialize();
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		lib::SocketAsync* socket = session->get_socket();

		BaseReceiver::register_client(id_in, session);
		//if (FALSE == _iocp->associate(id_in, *socket))
		//{
		//	return result;
		//}

		result = socket->init_client(address_in, port_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		result = socket->connect();
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}
//#if !defined(USE_ADVANCED_NETWORK)
//		result = socket->connect();
//		if (ERR_SUCCEEDED != result)
//		{
//			return result;
//		}
//#else
//		result = socket->advanced_connect();
//		if (ERR_SUCCEEDED != result)
//		{
//			return result;
//		}
//#endif
		//result = session->receive();
		//if (ERR_SUCCEEDED != result)
		//{
		//	return result;
		//}

		LOG(DEBUG, "connected new connection!!(%d)", id_in);

		if (ERR_SUCCEEDED != session->send((BYTE*)"hello", 6))
		{
			LOG(DEBUG, "failed send message!!");
		}

		_session_map.insert(TSessionMap::value_type(id_in, session));

		return ERR_SUCCEEDED;
	}

	//////////////////////////////////////////////////////////////////////////////////
	////
	//////////////////////////////////////////////////////////////////////////////////
	//INT32	BaseConnector::worker_thread()
	//{
	//	while (TRUE)
	//	{
	//		INT32 key = 0;
	//		UINT32 transferred_bytes = 0;
	//		LPOVERLAPPED overlapped = nullptr;

	//		INT32 result = _iocp->pop_income(key, transferred_bytes, &overlapped);
	//		if (ERR_SUCCEEDED != result)
	//		{
	//			LOG(DEBUG, "disconnected connection!!(%d)", key);
	//			continue;
	//		}

	//		TSessionMap::iterator iter = _session_map.find(key);
	//		if (iter == _session_map.end())
	//		{
	//			continue;
	//		}

	//		Session* session = iter->second;

	//		switch (((lib::SocketAsync::AsyncOverlapped*)overlapped)->get_type())
	//		{
	//		case lib::EO_ACCEPT:
	//			LOG(DEBUG, "accepted session(%d)!!", key);
	//			break;
	//		case lib::EO_RECEIVE:
	//			LOG(DEBUG, "received session(%d)!!", key);
	//			break;
	//		case lib::EO_SEND:
	//			LOG(DEBUG, "sended session(%d)!!", key);
	//			continue;
	//		case lib::EO_SHUTDOWN:
	//			return ERR_SUCCEEDED;
	//		default:
	//			LOG(DEBUG, "default(%d)!!", key);
	//			break;
	//		}

	//		//session->process(key);
	//		lib::MessageBuffer* message = session->get_message();
	//		LOG(DEBUG, "received message: %s", message->get_buffer());

	//		if (ERR_SUCCEEDED != session->send((BYTE*)"hello", 6))
	//		{
	//			LOG(DEBUG, "failed send message!!");
	//		}

	//		result = session->receive();
	//		if (ERR_SUCCEEDED != result)
	//		{
	//			LOG(DEBUG, "Failed to receive automatically!!(%d)", session->get_id());
	//			continue;
	//		}
	//	}

	//	return ERR_SUCCEEDED;
	//}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID BaseConnector::process_incomes(INT32 key_in, UINT32 bytes_in)
	{
		TSessionMap::iterator iter = _session_map.find(key_in);
		if (iter == _session_map.end())
		{
			return;
		}

		bytes_in;
		Session* session = iter->second;

		//session->process(key);
		lib::MessageBuffer* message = session->get_message();
		LOG(DEBUG, "received message: %s", message->get_buffer());

		if (ERR_SUCCEEDED != session->send((BYTE*)"hello", 6))
		{
			LOG(DEBUG, "failed send message!!");
		}

		INT32 result = session->receive();
		if (ERR_SUCCEEDED != result)
		{
			LOG(DEBUG, "Failed to receive automatically!!(%d)", session->get_id());
			return;
		}
	}
}