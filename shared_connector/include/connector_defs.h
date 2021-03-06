// @file	connector_defs.h
//
//

#ifndef _CONNECTOR_DEFS_H_
#define _CONNECTOR_DEFS_H_

#pragma once

#include "shared_defs.h"

#if defined(SHARED_CONNECTOR_EXPORTS)
	#define SHARED_CONNECTOR_API	__declspec(dllexport)
#else
	#define SHARED_CONNECTOR_API	__declspec(dllimport)
#endif

#endif
