//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "../config.h"
namespace SYNC
{

class SOCKETLIB_API CProcessMutex
{
private:
    CProcessMutex(const CProcessMutex &);
    void operator= (const CProcessMutex &);

public:
	CProcessMutex();
	~CProcessMutex();

	CProcessMutex(BOOL bInitialOwner);
	CProcessMutex(LPSECURITY_ATTRIBUTES pSecurity, BOOL bInitialOwner, LPCTSTR pszName);

	// Create a new mutex
	bool  Create( LPSECURITY_ATTRIBUTES pSecurity, BOOL bInitialOwner, LPCTSTR pszName );
	// Open an existing named mutex
	bool  Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName);
	// Release ownership of the mutex
	bool  Release();

	// Wait mutex
	DWORD Wait(DWORD nTimeOut);	

	// Ëø
	bool  Lock(DWORD nTimeOut);
	bool  Lock();
	bool  UnLock();
	bool  TryLock();

	HANDLE GetHandle() const;

private:
	bool Clear();

private:
    HANDLE m_hMutex;

};

};