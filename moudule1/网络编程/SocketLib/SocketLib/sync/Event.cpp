//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "event.h"
namespace SYNC
{

inline CEvent::CEvent(void)
	: m_hEvent(NULL)
{
}

inline CEvent::CEvent(BOOL bManualReset, BOOL bInitialState)
	: m_hEvent(NULL)

{
	Create(NULL, bManualReset, bInitialState, NULL);
};

inline CEvent::CEvent(LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName)
	: m_hEvent(NULL)
{
	Create(pSecurity, bManualReset, bInitialState, pszName);
};

inline CEvent::~CEvent(void)
{
	Clear();
}

inline bool CEvent::Create( LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName)
{
	Clear();
	m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, pszName);
	return (m_hEvent != NULL);
};

inline bool CEvent::Open( DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
{
	HANDLE hEvent = ::OpenEvent(dwAccess, bInheritHandle, pszName);
	if (hEvent == NULL)
		return false;

	if (hEvent != m_hEvent)
	{
		Clear();
		m_hEvent = hEvent;
	}
	return true;
};

inline bool CEvent::Pulse()
{
	return (::PulseEvent(m_hEvent));
};

inline bool CEvent::Reset()
{
	return (::ResetEvent(m_hEvent));
};

inline bool CEvent::Signal()
{
	return (::SetEvent(m_hEvent));
};

inline bool CEvent::Clear()
{
	if (m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
	return true;
};

inline DWORD CEvent::Wait(DWORD nTimeOut)
{
	return WaitForSingleObject(m_hEvent, nTimeOut);
};

inline bool CEvent::Lock(DWORD nTimeOut)
{
	DWORD hRet = Wait(nTimeOut);
	if (WAIT_OBJECT_0 == hRet)
		return true;
	return false;
};

inline bool CEvent::Lock()
{
	DWORD hRet = Wait(INFINITE);
	if (WAIT_OBJECT_0 == hRet)
		return true;
	return false;
};

inline bool CEvent::UnLock()
{
	return Signal();
};

inline bool CEvent::TryLock()
{
	return false;
};

HANDLE CEvent::GetHandle() const
{
	return m_hEvent;
}

};