// @file	base_receiver.cpp
//
//

#include "base_receiver.h"
#include "session.h"

#include "iocp.h"

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseReceiver::BaseReceiver()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	BaseReceiver::~BaseReceiver()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseReceiver::initialize(SHORT thread_num_in, std::vector<lib::Iocp*>* iocp_list_in)
	{
		INT32 result = Thread::register_thread(string("receiver_worker_thread"), &BaseReceiver::worker_thread, thread_num_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		TIdMap::iterator iter = _id_map.find(string("receiver_worker_thread"));
		if (iter == _id_map.end())
		{
			close();
			return -1;
		}

		if (nullptr == iocp_list_in)
		{
			for (auto& thread_id : iter->second)
			{
				lib::Iocp* iocp = new lib::Iocp();
				iocp->initialize();

				_iocp_map.insert(TIocpMap::value_type(thread_id, iocp));
				_iocp_list.push_back(iocp);
			}
		}
		else
		{
			_iocp_list.insert(_iocp_list.end(), iocp_list_in->begin(), iocp_list_in->end());

			INT32 index = 0;
			for (auto& thread_id : iter->second)
			{
				_iocp_map.insert(TIocpMap::value_type(thread_id, _iocp_list[index++]));
			}
		}
		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID BaseReceiver::close()
	{
		
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseReceiver::register_client(INT32 key_in, Session * session_in)
	{
		lib::Iocp* iocp = _iocp_list[key_in % Thread::get_thread_count(string("receiver_worker_thread"))];

		if (FALSE == iocp->associate(key_in, *session_in->get_socket()))
		{
			return ERR_SHARED_RECEIVER_REGISTRY;
		}
		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 BaseReceiver::worker_thread()
	{
		DWORD thread_id = GetCurrentThreadId();

		TIocpMap::iterator iter = _iocp_map.find(thread_id);
		if (iter == _iocp_map.end())
		{
			return -1;
		}

		lib::Iocp* current_iocp = iter->second;

		while (TRUE)
		{
			INT32 key = 0;
			UINT32 transferred_bytes = 0;
			LPOVERLAPPED overlapped = nullptr;

			INT32 result = current_iocp->pop_income(key, transferred_bytes, &overlapped);
			if (ERR_SUCCEEDED != result)
			{
				LOG(DEBUG, "disconnected client(%d)!!", key);
				continue;
			}

			switch (((lib::SocketAsync::AsyncOverlapped*)overlapped)->get_type())
			{
			case lib::EO_ACCEPT:
				LOG(DEBUG, "accepted session(%d)!!", key);
				break;
			case lib::EO_RECEIVE:
				LOG(DEBUG, "received session(%d)!!", key);
				process_incomes(key, transferred_bytes);
				break;
			case lib::EO_SEND:
				LOG(DEBUG, "sended session(%d)!!", key);
				continue;
			case lib::EO_SHUTDOWN:
				return ERR_SUCCEEDED;
			default:
				LOG(DEBUG, "default(%d)!!", key);
				break;
			}

			

			//Session* session = get_session(key);
			//if (nullptr == session)
			//{
			//	LOG(DEBUG, "Failed to find session(%d)!!", key);
			//	continue;
			//}

			//// process packet
			////session->process(key);

			//lib::MessageBuffer* message = session->get_message();
			//LOG(DEBUG, "received message: %s", message->get_buffer());

			////if (ERR_SUCCEEDED != session->send((BYTE*)"world", 6))
			////{
			////	LOG(DEBUG, "Failed to send message!!(%d)", session->get_id());
			////	disconnect(key);
			////	continue;
			////}

			//// reset session
			//result = session->receive();
			//if (ERR_SUCCEEDED != result)
			//{
			//	LOG(DEBUG, "Failed to receive automatically!!(%d)", session->get_id());
			//	continue;
			//}
		}
		return ERR_SUCCEEDED;
	}

}