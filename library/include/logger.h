// @file	logger.h
//
//

#ifndef _LOGGER_H_
#define _LOGGER_H_

#pragma once

#include "lib_defs.h"

#include <iostream>
#include <ctime>

enum ELOG_LEVEL
{
	  DEBUG = 0
	, INFO
	, WARNING
	, CRITICAL
};
namespace lib
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	Logger
	//
	// @brief	시스템 내에서 사용되는 로그들을 담당한다.
	//			
	////////////////////////////////////////////////////////////////////////////////
	class Logger
	{
	public:

		static VOID		log_debug(const CHAR* string_in, const CHAR* function_in, INT32 line_in)
		{
			time_t raw_time;
			std::tm time_info;
			CHAR buffer[80] = { 0, };

			time(&raw_time);

			localtime_s(&time_info, &raw_time);
			strftime(buffer, 80, "%Y-%m-%d %I:%M:%S", &time_info);

			std::cout << buffer << " : " << string_in << ". (" << function_in << ":" << line_in << ")" << std::endl;
		}

	};
}


#define	LOG(level, format, ...) \
{	\
	CHAR buffer[1024] = {0,}; \
	sprintf_s(buffer, 1024, format, __VA_ARGS__); \
	lib::Logger::log_debug(buffer, __FUNCTION__, __LINE__); \
}

#endif