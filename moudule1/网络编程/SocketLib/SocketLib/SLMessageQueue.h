//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef SLMESSAGE_QUEUE_H
#define SLMESSAGE_QUEUE_H

#pragma once
#include "config.h"
#include "slext/bytebuffer.h"
#include "sync/guard.h"

namespace SL
{

class SOCKETLIB_API CSLMessageQueueNode
{
public:
	CSLMessageQueueNode();
	~CSLMessageQueueNode();

public:

	SLEXT::CByteBuffer	 msg;
	int					 attachID;
	void			    *attachInfo;
	CSLMessageQueueNode *next;

};

template <class TMutex>
class CSLMessageQueue
{
public:
	CSLMessageQueue(void)
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_nCount = 0;
	};
	virtual ~CSLMessageQueue(void)
	{
		Clear();
	};
	uint32 GetSize()
	{
		SYNC::CGuard<TMutex> guard(m_lock);
		return m_nCount;
	};
	bool IsEmpty()
	{
		SYNC::CGuard<TMutex> guard(m_lock);
		return m_nCount==0;
	}
	bool Clear()
	{
		m_lock.Lock();
		if (m_pHead == NULL)
		{
			m_lock.UnLock();
			return false;
		}

		CSLMessageQueueNode *pNode = m_pHead;
		CSLMessageQueueNode *pNext;
		while (pNode != NULL)
		{
			pNext = pNode->next;
			delete pNode;
			pNode = pNext;
		};
		m_pHead = NULL;
		m_pTail = NULL;
		m_nCount = 0;
		m_lock.UnLock();

		return true;
	};

	int Enqueue(const char *szMsg, int nLen, int nAttachID, void *pAttachInfo)
	{
		m_lock.Lock();
		CSLMessageQueueNode *pNode = new CSLMessageQueueNode();
		if (pNode == NULL)
		{
			m_lock.UnLock();
			return -1;
		}

		pNode->msg.data(szMsg,nLen,true);
		pNode->attachID = nAttachID;
		pNode->attachInfo = pAttachInfo;
		if (m_pTail != NULL)
		{
			m_pTail->next	= pNode;
			m_pTail			= pNode;
		}
		else
		{
			m_pHead = pNode;
			m_pTail = pNode;
		}
		m_nCount++;
		m_lock.UnLock();

		return 0;
	};
	int Dequeue(CSLMessageQueueNode &oNode)
	{
		m_lock.Lock();
		if (m_pHead == NULL)
		{
			m_lock.UnLock();
			return -1;
		}

		oNode.attachID		= m_pHead->attachID;
		oNode.attachInfo	= m_pHead->attachInfo;
		oNode.msg.data(m_pHead->msg);
		oNode.next = 0;
		
		CSLMessageQueueNode *pTmp = m_pHead;
		m_pHead = m_pHead->next;
		delete pTmp;
		m_nCount--;
		if (m_nCount == 0)
		{
			m_pHead = NULL;
			m_pTail = NULL;
		}
		m_lock.UnLock();

		return 0;
	};
	
protected:
	CSLMessageQueueNode	*m_pHead;
	CSLMessageQueueNode	*m_pTail;
	uint32				 m_nCount;

	//线程安全锁
	TMutex		 m_lock;
};

};

#endif