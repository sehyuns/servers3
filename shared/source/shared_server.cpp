// @file	shared_server.cpp
//
//

#include "shared_server.h"


namespace shared
{
	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	SharedServer::SharedServer()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	SharedServer::~SharedServer()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SharedServer::initialize(const string & address_in, SHORT port_in, TSessionList* list_in, SHORT accept_num, SHORT worker_num, SHORT connector_num)
	{
		INT32 result = BaseAccepter::initialize(address_in, port_in, accept_num, worker_num);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		result = BaseConnector::initialize(connector_num);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		result = BaseAccepter::register_acceptors(list_in);
		if (ERR_SUCCEEDED != result)
		{
			return result;
		}

		return ERR_SUCCEEDED;
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	VOID SharedServer::close()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////////////////////////
	INT32 SharedServer::start_server()
	{
		INT32 result = BaseAccepter::start_accepter();
		if (ERR_SUCCEEDED == result)
		{
			return result;
		}

		result = BaseConnector::start_connector();
		if (ERR_SUCCEEDED == result)
		{
			return result;
		}
		
		return ERR_SUCCEEDED;
	}

}