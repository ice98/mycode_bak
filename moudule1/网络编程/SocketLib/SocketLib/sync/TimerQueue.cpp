//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "../stdafx.h"
#include "timerqueue.h"
namespace SYNC
{

inline CTimerQueue::CTimerQueue(void)
	: m_hTimerQueue(NULL)
{
	Create();
};

inline CTimerQueue::~CTimerQueue(void)
{
	Clear();
};

inline bool CTimerQueue::Clear()
{
	return Release(NULL);
};

inline bool CTimerQueue::Create()
{
	Clear();
	m_hTimerQueue = ::CreateTimerQueue();
	return (m_hTimerQueue != NULL);
};

inline bool CTimerQueue::Release(HANDLE hCompletionEvent)
{
	if (m_hTimerQueue != NULL)
	{
		BOOL bRet = ::DeleteTimerQueueEx(m_hTimerQueue,hCompletionEvent);
		m_hTimerQueue = NULL;
		return bRet;
	};
	return true;
};

//È¡µÃ¾ä±ú
inline HANDLE CTimerQueue::GetHandle() const
{
	return m_hTimerQueue;
};

inline bool CTimerQueue::CreateTimer(PHANDLE phNewTimer,TimerQueueProc timerQueueProc, PVOID pParameter, DWORD nDueTime, DWORD nPeriod, ULONG nFlags)
{
	return ::CreateTimerQueueTimer(phNewTimer,m_hTimerQueue,timerQueueProc,pParameter,nDueTime,nPeriod,nFlags);
};

bool CTimerQueue::DeleteTimer(HANDLE hTimer,HANDLE hCompletionEvent)
{
	return ::DeleteTimerQueueTimer(m_hTimerQueue,hTimer,hCompletionEvent);
};

bool CTimerQueue::UpdateTimer(HANDLE hTimer,ULONG nDueTime,ULONG nPeriod)
{
	return ::ChangeTimerQueueTimer(m_hTimerQueue,hTimer,nDueTime,nPeriod);
};

};