/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* 文件名称：TcpServer.h
* 摘    要：TcpServer
* 作    者：张保元 bolidezhang@gmail.com
* 完成日期：2005/07/16
*
*/

#pragma once
#include <map>
#include <list>
#include "SocketSource.h"
#include "SocketLib.h"
#include "sync/Thread.h"

namespace SL
{

class SOCKETLIB_API CTcpServer : public CSocketSource
{
public:
	CTcpServer(void);
	virtual ~CTcpServer(void);

	void SetConfig(int nMaxSocketNum, int nMaxFreePoolNum, int nDefaultFreePoolNum);
	void SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner);
	void SetBufferSize(int nRecvBufferSize, int nSendBufferSize);

	bool Open(const char *szServerName, unsigned short nPort, int nBacklog = 10);
	bool Close();

	bool IsConnected(SOCKET hClientSocket);
	bool Disconnect(SOCKET hClientSocket, bool bForce = false);
	int  Send(SOCKET hClientSocket, const char *szData, int nLen);

	//socket的附加信息指针
	bool  SetAttachInfo(SOCKET hClientSocket, void *pAttachInfo);
	void* GetAttachInfo(SOCKET hClientSocket);

protected:
	static unsigned int WINAPI ListenProc(LPVOID lParam);

	bool PostRecv(SOCKET hClientSocket, void *p);
	int  Recv(SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen);

protected:

	SOCKET			m_hListenSocket;

	//开始标记(控制侦听线程,使其自动结束)
	bool			m_bIsStart;

	//临界保护量
	CRITICAL_SECTION m_lock;

	//当前在线连接
	std::map<SOCKET,PSOCKET_SESSION>		m_mapSocketSession;

	//空闲对象池
	std::list<PSOCKET_SESSION>				m_lstFreePool;

	//Socket最大连接数
	int	m_nMaxSocketNum;
	//最大空闲池的数量
	int m_nMaxFreePoolNum;
	//默认空闲池的数量
	int m_nDefaultFreePoolNum;

	//listen线程
	SYNC::CThread m_threadListen;

};

};
