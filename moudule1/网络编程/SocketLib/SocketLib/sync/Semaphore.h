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

class SOCKETLIB_API CSemaphore
{
private:
	// = Prevent copying.
	CSemaphore (const CSemaphore& oSemaphore);
	CSemaphore &operator= (const CSemaphore &oSemaphore);
public:
	CSemaphore(void);
	CSemaphore(LONG nInitialCount, LONG nMaxCount);
	CSemaphore(LPSECURITY_ATTRIBUTES pSecurity, LONG nInitialCount, LONG nMaxCount, LPCTSTR pszName);
	virtual ~CSemaphore(void);

	// Create a new semaphore
	bool Create(LPSECURITY_ATTRIBUTES pSecurity, LONG nInitialCount, LONG nMaxCount, LPCTSTR pszName);
	// Open an existing named semaphore
	bool Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName);
	// Increase the count of the semaphore
	bool Release(LONG nReleaseCount = 1, LONG* pnOldCount = NULL);

	// Wait Semaphore
	DWORD Wait(DWORD nWaitTime);

	// Ëø
	bool  Lock(DWORD nTimeOut);
	bool  Lock();
	bool  UnLock();
	bool  TryLock();

	HANDLE GetHandle() const;

private:
	bool Clear();

protected:
	HANDLE	m_hSemaphore;
};

};