// @file	locker.h
//
//

#ifndef _LOCKER_H_
#define _LOCKER_H_

#pragma once

#include "lib_defs.h"

namespace lib
{
	////////////////////////////////////////////////////////////////////////////////
	// @class	AutoLocker
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class AutoLocker
	{
	public:
		AutoLocker(T* locker_in)
			: _locker(locker_in)
		{
			_locker->lock();
		}

		AutoLocker(T& locker_in)
			: _locker(&locker_in)
		{
			_locker->lock();
		}

		~AutoLocker()
		{
			_locker->unlock();
		}

	private:
		T*		_locker;
	};


	////////////////////////////////////////////////////////////////////////////////
	// @class	ScopedLocker
	//
	// @brief
	//			
	////////////////////////////////////////////////////////////////////////////////
	class ScopedLocker
	{
	public:
		ScopedLocker();
		virtual ~ScopedLocker();

	public:
		VOID			lock();
		VOID			unlock();

		BOOL			try_lock();

	private:
		CRITICAL_SECTION	_locker;
	};
}

#endif
