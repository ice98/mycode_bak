/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* 文件名称：TcpClient.h
* 摘    要：TcpClient
* 作    者：张保元 bolidezhang@gmail.com
* 完成日期：2005/07/16
*
*/

#pragma once
#include <map>
#include <list>
#include <string>
#include "SocketSource.h"
#include "SocketLib.h"
#include "sync/threadmutex.h"

namespace SL
{

class SOCKETLIB_API CTcpClient : public CSocketSource
{
public:
	CTcpClient(void);
	virtual ~CTcpClient(void);

	void SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner);
	void SetBufferSize(int nRecvBufferSize, int nSendBufferSize);

	virtual bool DoOpen();
	bool Open(const char *szServerName, unsigned short nServerPort, int nLocalPort = 0);
	bool Close();

	bool IsConnected() const;
	bool Disconnect(SOCKET hSocket, bool bForce = false);
	int  Send(const char *szData, int nLen, bool bConnected = true);

	SOCKET GetSocketHandle() const;
protected:
	bool PostRecv(SOCKET hSocket, void *p);
	int  Recv(SOCKET hSocket, void *pAttachInfo, const char *szData, int nLen);

protected:
	SOCKET m_hSocket;

	// 连接状态
	bool   m_bIsConnected;

	//锁
	SYNC::CThreadMutex m_lock;

	SOCKET_SESSION   m_SocketSession;
	std::string		 m_strServerName;    //远程服务器名字:可以是IP地址(218.1.65.74)或域名(www.cga.com.cn)
	unsigned short   m_nServerPort;    
	unsigned short   m_nLocalPort;
};

};
