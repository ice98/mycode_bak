//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "semaphore.h"
namespace SYNC
{

inline CSemaphore::CSemaphore(void)
	: m_hSemaphore(NULL)
{
}

inline CSemaphore::CSemaphore(LONG nInitialCount, LONG nMaxCount)
	: m_hSemaphore(NULL)
{
	Create(NULL, nInitialCount, nMaxCount, NULL);
}

inline CSemaphore::CSemaphore(LPSECURITY_ATTRIBUTES pSecurity, LONG nInitialCount, LONG nMaxCount, LPCTSTR pszName)
	: m_hSemaphore(NULL)
{
	Create(pSecurity, nInitialCount, nMaxCount, pszName);
}

inline CSemaphore::~CSemaphore(void)
{
	Clear();
}

inline bool CSemaphore::Create(LPSECURITY_ATTRIBUTES pSecurity, LONG nInitialCount, LONG nMaxCount, LPCTSTR pszName ) throw()
{
	m_hSemaphore = ::CreateSemaphore(pSecurity, nInitialCount, nMaxCount, pszName);
	return (m_hSemaphore != NULL);
}

inline bool CSemaphore::Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName ) throw()
{
	HANDLE hSemaphore = ::OpenSemaphore(dwAccess, bInheritHandle, pszName);
	if (hSemaphore == NULL)
		return false;

	if (hSemaphore != m_hSemaphore)
	{
		Clear();
		m_hSemaphore = hSemaphore;
	}
	return true;
}

inline bool CSemaphore::Release(LONG nReleaseCount, LONG* pnOldCount)
{
	return (::ReleaseSemaphore(m_hSemaphore, nReleaseCount, pnOldCount));
}

inline bool CSemaphore::Clear()
{
	if (m_hSemaphore != NULL)
	{
		CloseHandle(m_hSemaphore);
		m_hSemaphore = NULL;
	}
	return true;
};

inline DWORD CSemaphore::Wait(DWORD nWaitTime)
{
	return WaitForSingleObject(m_hSemaphore, nWaitTime);
};

inline bool CSemaphore::Lock(DWORD nTimeOut)
{
	DWORD nRet = Wait(nTimeOut);
	if (WAIT_OBJECT_0 == nRet)
		return true;
	return false;
};

inline bool CSemaphore::Lock()
{
	DWORD nRet = Wait(INFINITE);
	if (WAIT_OBJECT_0 == nRet)
		return true;
	return false;
};

inline bool CSemaphore::UnLock()
{
	LONG nOldCount = 0;	
	return Release(1,&nOldCount);
};

inline bool CSemaphore::TryLock()
{
	return false;
};

inline HANDLE CSemaphore::GetHandle() const
{
	return m_hSemaphore;
};


};