// @file	winsock_env.h
//
//

#ifndef _WINSOCK_ENV_H_
#define _WINSOCK_ENV_H_

#pragma once

#include "lib_defs.h"

namespace lib
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	WinsockEnv
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	class WinsockEnv
	{
	public:
		WinsockEnv()
		{
			WSADATA wsa_data;
			WSAStartup(MAKEWORD(2, 2), &wsa_data);
		}

		~WinsockEnv()
		{
			WSACleanup();
		}
	};
}

#endif
