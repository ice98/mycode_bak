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

class SOCKETLIB_API CEvent
{
private:
	// = Prevent copying.
	CEvent (const CEvent& oEvent);
	CEvent &operator= (const CEvent &oEvent);
public:
	CEvent(void);
	virtual ~CEvent(void);

	CEvent(BOOL bManualReset, BOOL bInitialState);
	CEvent(LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName);

	// Create a new event
	bool  Create(LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName);
	// Open an existing named event
	bool  Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName);
	// Pulse the event (signals waiting objects, then resets)
	bool  Pulse();
	// Set the event to the non-signaled state
	bool  Reset();
	// Set the event to the signaled state
	bool  Signal();

	// Wait event
	DWORD Wait(DWORD nTimeOut);	

	// Ëø
	bool  Lock(DWORD nTimeOut);
	bool  Lock();
	bool  UnLock();
	bool  TryLock();

	//È¡µÃ¾ä±ú
	HANDLE GetHandle() const;

private:
	bool Clear();

protected:
	HANDLE	m_hEvent;

};

};