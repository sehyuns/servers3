// @file	client_connector.h
//
//

#ifndef _CLIENT_CONNECTOR_H_
#define _CLIENT_CONNECTOR_H_

#pragma once

#include "winsock_env.h"
#include "base_connector.h"

namespace connector
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	ClientConnector
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	class ClientConnector : public shared::BaseConnector, public lib::WinsockEnv
	{
	public:
		ClientConnector();
		~ClientConnector();

	private:
	};
}

#endif
