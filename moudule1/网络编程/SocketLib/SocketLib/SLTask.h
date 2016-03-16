//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef SLTASK_H
#define SLTASK_H

#pragma once
#include "config.h"
#include "slmessagequeue.h"
#include "sync/threadgroup.h"
#include "sync/semaphore.h"

namespace SL
{

template <class TMutex>
class CSLTask
{
public:
	CSLTask(void)
		: m_nMaxQueueSize(100)
		, m_semaphore(0, 100)
	{
	};
	CSLTask(uint32 nMaxCount)
		: m_nMaxQueueSize(nMaxCount)
		, m_semaphore(0, nMaxCount)
	{
	};
	virtual ~CSLTask(void)
	{
		Close();
	};

	void SetMaxQueueSize(uint32 nMaxSize)
	{
		m_nMaxQueueSize = nMaxSize;
	}
	uint32 GetMsgQueueSize()
	{
		return m_msgQueue.GetSize();
	};
	int GetMessage(CSLMessageQueueNode& oNode)
	{
		return m_msgQueue.Dequeue(oNode);
	};
	int PostMessage(const char *szMsg, int nLen, int nAttachID, void *pAttachInfo)
	{
		if (m_msgQueue.Enqueue(szMsg, nLen, nAttachID, pAttachInfo) >= 0)
			m_semaphore.UnLock();
		else
			return -1;
		return 1;
	};
	int Pop(CSLMessageQueueNode& oNode)
	{
		return m_msgQueue.Dequeue(oNode);
	};
	int Push(const char *szMsg, int nLen, int nAttachID, void *pAttachInfo)
	{
		return m_msgQueue.Enqueue(szMsg, nLen, nAttachID, pAttachInfo);
	};
	virtual bool Open(void *p) 
	{
		return true;
	};
	virtual bool Close() 
	{
		m_threadGroup.Kill();
		m_msgQueue.Clear();
		m_semaphore.Clear();
		return true;
	};
	int Activate(int nThreadCount)
	{
		return m_threadGroup.Start(CSLTask<TMutex>::Run, this, nThreadCount);
	};
	bool Resume()
	{
		return m_threadGroup.Resume();
	};
	bool Suspend()
	{
		return m_threadGroup.Suspend();
	};
	int GetThreadCount() const
	{
		return m_threadGroup.GetThreadCount();
	};

	virtual int ProcessMessage(CSLMessageQueueNode& oNode)
	{
		return 0;
	};

private:
	// Routine that runs the task routine as a daemon thread.
	static unsigned int WINAPI Run(void *p)
	{
		CSLTask<TMutex> *pTask =(CSLTask<TMutex>*)p;
		CSLMessageQueueNode oNode;
		while (!pTask->m_treadGroup.IsStop())
		{
			pTask->m_semaphore.Lock();
			if (pTask->GetMessage(oNode) >= 0)
			{//取到处理消息时
				pTask->ProcessMessage(oNode);
			}
		};
		return 0;
	};

protected:
	CSLMessageQueue<TMutex> m_msgQueue;
	SYNC::CThreadGroup		m_threadGroup;
	SYNC::CSemaphore		m_semaphore;
	uint32					m_nMaxQueueSize;
};

};

#endif