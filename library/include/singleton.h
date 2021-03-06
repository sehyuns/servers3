// @file	singleton.h
//
//

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#pragma once

namespace lib
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	Singleton
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class Singleton
	{
	public:
		static T	get_instance()
		{
			static T	instance;
			return instance;
		}

	protected:
		Singleton() {}
		~Singleton() {}
	};
}

#endif
