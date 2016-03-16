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

typedef unsigned int(WINAPI *ThreadProc)(void *pParameter);

class SOCKETLIB_API CThread
{
public:

	enum ThreadCmd{nothing = 0, stop};
	enum ThreadState{norun = 0, normal, suspend};

	CThread(void);
	CThread(ThreadProc threadProc, void *m_pParameter, bool bSuspend = false);
	virtual ~CThread(void);

	bool Start(ThreadProc threadProc, void *m_pParameter, bool bSuspend = false, int nPriority = THREAD_PRIORITY_NORMAL);
	bool Start(void);

	//继续线程
	bool Resume(void);

	//挂起线程
	bool Suspend(void);

	//停止线程(友好退出)
	bool  IsStop() const;
	bool  Stop(void);
	DWORD Stop(DWORD nTimeOut);
	//等待线程退出
	DWORD Wait(DWORD nTimeOut);

	//强制线程停止(这时线程不作清理资源或内存,可能会引起资源或内存泄漏)
	bool Kill(void);

	HANDLE GetHandle() const;

	int  GetPriority();
	bool SetPriority(int nPriority);

protected:
	bool Clear();

protected:

	//线程句柄
	HANDLE		m_hThread;

	ThreadProc  m_threadProc;
	void	   *m_pParameter;

	ThreadCmd   m_ThreadCmd;
	ThreadState	m_ThreadState;
};

};