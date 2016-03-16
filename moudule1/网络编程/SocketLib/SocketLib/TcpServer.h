/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* �ļ����ƣ�TcpServer.h
* ժ    Ҫ��TcpServer
* ��    �ߣ��ű�Ԫ bolidezhang@gmail.com
* ������ڣ�2005/07/16
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

	//socket�ĸ�����Ϣָ��
	bool  SetAttachInfo(SOCKET hClientSocket, void *pAttachInfo);
	void* GetAttachInfo(SOCKET hClientSocket);

protected:
	static unsigned int WINAPI ListenProc(LPVOID lParam);

	bool PostRecv(SOCKET hClientSocket, void *p);
	int  Recv(SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen);

protected:

	SOCKET			m_hListenSocket;

	//��ʼ���(���������߳�,ʹ���Զ�����)
	bool			m_bIsStart;

	//�ٽ籣����
	CRITICAL_SECTION m_lock;

	//��ǰ��������
	std::map<SOCKET,PSOCKET_SESSION>		m_mapSocketSession;

	//���ж����
	std::list<PSOCKET_SESSION>				m_lstFreePool;

	//Socket���������
	int	m_nMaxSocketNum;
	//�����гص�����
	int m_nMaxFreePoolNum;
	//Ĭ�Ͽ��гص�����
	int m_nDefaultFreePoolNum;

	//listen�߳�
	SYNC::CThread m_threadListen;

};

};
