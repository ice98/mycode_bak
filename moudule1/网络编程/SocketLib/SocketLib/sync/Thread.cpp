//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\thread.h"
#include <process.h>
namespace SYNC
{

CThread::CThread(void)
	: m_hThread(NULL)
	, m_ThreadCmd(ThreadCmd::nothing)
	, m_ThreadState(ThreadState::norun)
{
}

CThread::CThread(ThreadProc threadProc, void *pParameter, bool bSuspend)
	: m_hThread(NULL)
	, m_ThreadCmd(ThreadCmd::nothing)
	, m_ThreadState(ThreadState::norun)
{
	Start(threadProc,pParameter,bSuspend);
}

CThread::~CThread(void)
{
	Clear();
}

bool CThread::Start(ThreadProc threadProc, void *pParameter, bool bSuspend, int nPriority)
{
	Clear();
	unsigned int nThreadID = 0;
	if (bSuspend) 
		m_hThread = (HANDLE)_beginthreadex(NULL,0,threadProc,pParameter,CREATE_SUSPENDED,&nThreadID);
	else
		m_hThread = (HANDLE)_beginthreadex(NULL,0,threadProc,pParameter,0,&nThreadID);
	
	if (m_hThread == NULL)
		return false;
	else
	{
		SetPriority(nPriority);
		m_threadProc  = threadProc;
		m_pParameter  = pParameter;
		if (bSuspend)
			m_ThreadState = ThreadState::suspend;
		else
			m_ThreadState = ThreadState::normal;
		return true;
	}
}

bool CThread::Start()
{
	Clear();
	unsigned int nThreadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL,0,m_threadProc,m_pParameter,0,&nThreadID);
	if (NULL == m_hThread)
		return false;
	else
	{
		m_ThreadState = ThreadState::normal;
		return true;
	}
}

inline bool CThread::Resume()
{
	DWORD nRet = ResumeThread(m_hThread);
	if (nRet == -1)
		return false;
	else
	{
		m_ThreadState = ThreadState::normal;
		return true;
	}
}

inline bool CThread::Suspend()
{
	DWORD nRet = SuspendThread(m_hThread);
	if (nRet == -1)
		return false;
	else
	{
		m_ThreadState = ThreadState::suspend;
		return true;
	}
}

inline bool CThread::Stop()
{
	m_ThreadCmd = ThreadCmd::stop;
	return true;
}

inline DWORD CThread::Stop(DWORD nTimeOut)
{
	m_ThreadCmd = ThreadCmd::stop;
	return Wait(nTimeOut);
}

inline DWORD CThread::Wait(DWORD nTimeOut)
{
	DWORD nRet = WaitForSingleObject(m_hThread, nTimeOut);
	if (nRet == WAIT_OBJECT_0)
		m_ThreadState = ThreadState::norun;
	return nRet;
}

inline bool CThread::Kill()
{
	BOOL bRet = TerminateThread(m_hThread,0);
	if (bRet)
	{
		m_ThreadState = ThreadState::norun;
		return true;
	}
	return false;
}

inline bool CThread::Clear()
{
	if (m_ThreadState != ThreadState::norun)
		Kill();
	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_ThreadCmd   = ThreadCmd::nothing;
	m_ThreadState = ThreadState::norun;
	return true;
}

inline HANDLE CThread::GetHandle() const
{
	return m_hThread;
}

inline bool CThread::IsStop() const
{
	if (m_ThreadCmd != ThreadCmd::stop)
		return false;
	else
		return true;
};

inline int CThread::GetPriority()
{
	return ::GetThreadPriority(m_hThread);
};

inline bool CThread::SetPriority(int nPriority)
{
	return ::SetThreadPriority(m_hThread, nPriority);
};

};