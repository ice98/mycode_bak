//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "processmutex.h"
namespace SYNC
{

inline CProcessMutex::CProcessMutex(void)
	: m_hMutex(NULL)
{
};

inline CProcessMutex::CProcessMutex(BOOL bInitialOwner)
{
	Create(NULL, bInitialOwner, NULL);
};

inline CProcessMutex::CProcessMutex(LPSECURITY_ATTRIBUTES pSecurity, BOOL bInitialOwner, LPCTSTR pszName)
{
	Create(pSecurity, bInitialOwner, pszName);
};

inline CProcessMutex::~CProcessMutex(void)
{
	Clear();
};

inline bool CProcessMutex::Clear()
{
	if (m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	return true;
};

inline bool CProcessMutex::Create( LPSECURITY_ATTRIBUTES pSecurity, BOOL bInitialOwner, LPCTSTR pszName)
{
	Clear();
	m_hMutex = ::CreateMutex( pSecurity, bInitialOwner, pszName);
	return (m_hMutex != NULL);
};

inline bool CProcessMutex::Open( DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
{
	HANDLE hMutex = ::OpenMutex(dwAccess, bInheritHandle, pszName);
	if (hMutex == NULL)
		return false;

	if (hMutex != m_hMutex)
	{
		Clear();
		m_hMutex = hMutex;
	}
	return true;
};

inline bool CProcessMutex::Release()
{
	return (::ReleaseMutex(m_hMutex));
};

inline DWORD CProcessMutex::Wait(DWORD nTimeOut)
{
	return WaitForSingleObject(m_hMutex, nTimeOut);
};

inline bool CProcessMutex::Lock(DWORD nTimeOut)
{
	DWORD hRet = Wait(nTimeOut);
	if (WAIT_OBJECT_0 == hRet)
		return true;
	return false;
};

inline bool CProcessMutex::Lock()
{
	DWORD hRet = Wait(INFINITE);
	if (WAIT_OBJECT_0 == hRet)
		return true;
	return false;
};

inline bool CProcessMutex::UnLock()
{
	return false;
};

inline bool CProcessMutex::TryLock()
{
	return false;
};

inline HANDLE CProcessMutex::GetHandle() const
{
	return m_hMutex;
};

};