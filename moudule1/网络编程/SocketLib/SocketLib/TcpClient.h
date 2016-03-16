/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* �ļ����ƣ�TcpClient.h
* ժ    Ҫ��TcpClient
* ��    �ߣ��ű�Ԫ bolidezhang@gmail.com
* ������ڣ�2005/07/16
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

	// ����״̬
	bool   m_bIsConnected;

	//��
	SYNC::CThreadMutex m_lock;

	SOCKET_SESSION   m_SocketSession;
	std::string		 m_strServerName;    //Զ�̷���������:������IP��ַ(218.1.65.74)������(www.cga.com.cn)
	unsigned short   m_nServerPort;    
	unsigned short   m_nLocalPort;
};

};
