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
#include "socketevent.h"
#include "sync/threadmutex.h"
#include "slext/smallobjectpool.h"

namespace SL
{

template <class TSession>
class CSLTcpSessionEvent : public CSocketEvent
{
public:
	CSLTcpSessionEvent(void)
	{
		m_sessionPool.init(1024,4,1);
	};

	virtual ~CSLTcpSessionEvent(void)
	{
		m_sessionPool.clear();
	};

	virtual bool OnAccept(CSocketSource *pSource, SOCKET hClientSocket)
	{
		return true;
	};

	virtual bool OnInitSocket(PPER_HANDLE_DATA pSocketData)
	{
		TSession *p = m_sessionPool.pop();
		if (p == NULL)
			return false;

		p->Init(pSocketData->pSocketSource,pSocketData->hSocket, 1024, 4);
		pSocketData->pAttachInfo = p;
		return true;
	};

	virtual bool OnDisconnect(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo)
	{
		if (pAttachInfo == NULL)
			return false;

		TSession *p = (TSession*)pAttachInfo;
		p->Uninit(); //ÇåÀí
		m_sessionPool.push(p);
		return true;
	};

	virtual int OnRecv(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen)
	{
		if (pAttachInfo == NULL)
			return 0;

		TSession *p = (TSession*)pAttachInfo;
		return p->OnRecv(szData, nLen);
	};

protected:
	SLEXT::CSmallObjectPool<TSession,SYNC::CThreadMutex> m_sessionPool;

};

};
