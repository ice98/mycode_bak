//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\threadgroup.h"
#include "guard.h"
#include <process.h>
namespace SYNC
{

CThreadGroup::CThreadGroup(void)
	: m_ThreadCmd(CThread::ThreadCmd::nothing)
	, m_threadProc(NULL)
	, m_pParameter(NULL)
{
	m_vecThreads.reserve(16);
};

CThreadGroup::CThreadGroup(ThreadProc threadProc, void *pParameter)
	: m_threadProc(threadProc)
	, m_pParameter(pParameter)
 	, m_ThreadCmd(CThread::ThreadCmd::nothing)
{
	m_vecThreads.reserve(16);
}

CThreadGroup::~CThreadGroup(void)
{
	Clear();
};

bool CThreadGroup::Start(ThreadProc threadProc, void *pParameter, int nThreadCount, int nPriority)
{	
	if (NULL == threadProc)
		return false;

	if (nThreadCount > 0)
	{
		Clear();
		unsigned int nThreadID = 0;
		HANDLE hThread	 = NULL;
		CGuard<CThreadMutex> guard(m_lock);
		for (int i=0; i<nThreadCount; i++)
		{
			hThread = (HANDLE)_beginthreadex(NULL,0,threadProc,pParameter,0,&nThreadID);
			if (NULL == hThread)
				return false;
			::SetThreadPriority(hThread, nPriority);
			m_vecThreads.push_back(hThread);
		}
	}
	m_threadProc = threadProc;
	m_pParameter = pParameter;

	return true;
}

bool CThreadGroup::Start(int nThreadCount)
{	
	if (NULL == m_threadProc)
		return false;

	if (nThreadCount > 0)
	{
		Clear();
		unsigned int nThreadID = 0;
		HANDLE hThread	 = NULL;
		CGuard<CThreadMutex> guard(m_lock);
		for (int i=0; i<nThreadCount; i++)
		{
			hThread = (HANDLE)_beginthreadex(NULL,0,m_threadProc,m_pParameter,0,&nThreadID);
			if (NULL == hThread)
				return false;
			m_vecThreads.push_back(hThread);
		}
	}
	return true;
}

inline void CThreadGroup::Stop(DWORD nTimeOut)
{
	{
		CGuard<CThreadMutex> guard(m_lock);
		m_ThreadCmd = CThread::ThreadCmd::stop;
	}
	if (nTimeOut > 0)
		Sleep(nTimeOut);
	return;
}

inline bool CThreadGroup::IsStop() const
{
	if (m_ThreadCmd != CThread::ThreadCmd::stop)
		return false;
	else
		return true;
};

bool CThreadGroup::Kill()
{
	return Clear();
}

bool CThreadGroup::Clear()
{
	HANDLE hThread;
	std::vector<HANDLE>::iterator itThread;
	CGuard<CThreadMutex> guard(m_lock);
	for (itThread = m_vecThreads.begin(); itThread != m_vecThreads.end(); itThread++)
	{
		hThread = *itThread;
		TerminateThread(hThread,0);
		CloseHandle(hThread);
	}
	m_vecThreads.clear();
	m_ThreadCmd = CThread::ThreadCmd::nothing;
	return true;
}

inline int CThreadGroup::GetThreadCount()
{
	CGuard<CThreadMutex> guard(m_lock);
	return (int)m_vecThreads.size();
};

int CThreadGroup::AddThread(int nThreadCount)
{
	if (NULL == m_threadProc)
		return 0;
	if (nThreadCount <= 0)
		return 0;

	int nRet = 0;
	unsigned int nThreadID = 0;
	HANDLE hThread	 = NULL;
	CGuard<CThreadMutex> guard(m_lock);
	for (int i=0; i<nThreadCount; i++)
	{
		hThread = (HANDLE)_beginthreadex(NULL,0,m_threadProc,m_pParameter,0,&nThreadID);
		if (NULL != hThread)
		{
			nRet++;
			m_vecThreads.push_back(hThread);
		}
	}
	return nRet;
};

int CThreadGroup::RemoveThread(int nThreadCount)
{
	if (nThreadCount>0)
	{
		int nRet = GetThreadCount();
		if (nThreadCount >= nRet)
			Clear();
		else
		{
			nRet = nThreadCount;
			CGuard<CThreadMutex> guard(m_lock);
			for (int i=0; i<nThreadCount; i++)
			{
				const HANDLE &hThread = m_vecThreads.back();
				TerminateThread(hThread,0);
				CloseHandle(hThread);
				m_vecThreads.pop_back();
			}
		}
		return nRet;
	}
	return 0;
};

bool CThreadGroup::Resume()
{
	HANDLE hThread;
	std::vector<HANDLE>::iterator itThread;
	CGuard<CThreadMutex> guard(m_lock);
	for (itThread = m_vecThreads.begin(); itThread != m_vecThreads.end(); itThread++)
	{
		hThread = *itThread;
		ResumeThread(hThread);
	};
	return true;
};

bool CThreadGroup::Suspend()
{
	HANDLE hThread;
	std::vector<HANDLE>::iterator itThread;
	CGuard<CThreadMutex> guard(m_lock);
	for (itThread = m_vecThreads.begin(); itThread != m_vecThreads.end(); itThread++)
	{
		hThread = *itThread;
		SuspendThread(hThread);
	};
	return true;
};

};