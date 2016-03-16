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
#include <list>
#include "SocketSource.h"
#include "SocketLib.h"
#include "sync/threadmutex.h"

namespace SL
{

class SOCKETLIB_API CUdpSource : public CSocketSource
{
public:
	CUdpSource();
	CUdpSource(int nMaxFreeRecvPoolNum, int nMaxFreeSendPoolNum);
	virtual ~CUdpSource(void);

	void SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner);
	void SetBufferSize(int nRecvBufferSize, int nSendBufferSize);

	bool Open(const char *szServerName, unsigned short nServerPort, int nLocalPort);
	bool Open(unsigned short nLocalPort = 0);
	bool Close();

	int  Send(const char *szData, int nLen, sockaddr *pTo, int nToLen);
	int  Send(const char *szData, int nLen, const char *szIP, unsigned short nPort);
	int  Send(const char *szData, int nLen);

protected:
	bool PostRecv(void *p);
	bool PostSend(void *p);
	int  Recv(const char *szData, int nLen, sockaddr *pFrom, int nFromLen);

	PPER_IO_OPERATION_BASE_UDP GetSendObj();
	void FreeSendObj(PPER_IO_OPERATION_BASE_UDP p);

	PPER_IO_OPERATION_DATA_UDP GetRecvObj();
	void FreeRecvObj(PPER_IO_OPERATION_DATA_UDP p);

protected:
	SOCKET m_hSocket;
	sockaddr_in		 m_ServerAddr;   //远程服务器地址
	PER_HANDLE_DATA  m_perHandle;

	//最大空闲池的数量
	int m_nMaxFreeRecvPoolNum;
	int m_nMaxFreeSendPoolNum;

	//存储池
	std::list<PPER_IO_OPERATION_DATA_UDP> m_lstFreeRecvPool;  //数据接收内存池
	std::list<PPER_IO_OPERATION_BASE_UDP> m_lstFreeSendPool;  //数据发送内存池

	//锁
	SYNC::CThreadMutex m_lock;
	SYNC::CThreadMutex m_lockRecv;
	SYNC::CThreadMutex m_lockSend;
};

};
