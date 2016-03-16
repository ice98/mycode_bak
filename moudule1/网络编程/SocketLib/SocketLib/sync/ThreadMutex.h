//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#pragma once
#include "../config.h"
namespace SYNC
{

class SOCKETLIB_API CThreadMutex
{
private:
    CThreadMutex(const CThreadMutex &);
    void operator= (const CThreadMutex &);

public:
	CThreadMutex()
	{
		InitializeCriticalSection(&m_mtx);
	};

	~CThreadMutex()
	{
		DeleteCriticalSection(&m_mtx);
	};

	inline DWORD Wait(DWORD nTimeOut)
	{
		return 0;
	};

	inline bool Lock(DWORD nTimeOut)
	{
		EnterCriticalSection(&m_mtx);
		return true;
	};

	inline bool Lock()
	{
		EnterCriticalSection(&m_mtx);
		return true;
	};

	inline bool UnLock()
	{
		LeaveCriticalSection(&m_mtx);
		return true;
	};

	inline bool TryLock()
	{
#if (_WIN32_WINNT >= 0x0400)
		return ::TryEnterCriticalSection(&m_mtx);
#else
		return false;
#endif		
	};

	inline HANDLE GetHandle() const
	{
		return (HANDLE)&m_mtx;
	};

private:
    CRITICAL_SECTION m_mtx;

};

};