// @file	shared_server.h
//
//

#ifndef _SHARED_SERVER_H_
#define _SHARED_SERVER_H_

#pragma once

#include "winsock_env.h"

#include "base_accepter.h"
#include "base_connector.h"

namespace shared
{
	class Session;
}

namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	SharedServer
	//
	// @brief	네트워크 기능을 가지는 가장 기본적인 서버 모델. 확장을 위해서는 이를 상속해서 원하는 모습의 서버를 구축할 수 있다.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class SharedServer : public BaseAccepter, public BaseConnector, public lib::WinsockEnv
	{
		typedef std::list<Session*>			TSessionList;

	public:
		SharedServer();
		virtual ~SharedServer();

	public:
		INT32			initialize(const string& address_in, SHORT port_in, TSessionList* list_in, SHORT accept_num=1, SHORT worker_num=1, SHORT connector_num=1);
		VOID			close();

		INT32			start_server();

	private:
	};
}

#endif
