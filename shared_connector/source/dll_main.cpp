// @file	dll_main.cpp
//
//

#include "connector_defs.h"

SHARED_CONNECTOR_API BOOL initialize()
{
	return TRUE;
}

SHARED_CONNECTOR_API VOID close()
{

}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	return TRUE;
}

