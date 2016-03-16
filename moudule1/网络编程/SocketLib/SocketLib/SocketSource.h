/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* �ļ����ƣ�SocketSource.h
* ժ    Ҫ��SocketԴ�ӿ���
* ��    �ߣ��ű�Ԫ bolidezhang@gmail.com
* ������ڣ�2005/07/16
*
*/

#pragma once
#include "config.h"
#include "AppLog.h"
#include "SocketRunner.h"
#include "SocketLib.h"
#include <string>

namespace SL
{

class CSocketEvent;
class SOCKETLIB_API CSocketSource
{
public:
	CSocketSource(void);
	virtual ~CSocketSource(void);

	virtual bool Open(const char *szServerName, unsigned short nPort, int nOption) = 0;
	virtual bool Close() = 0;

	//TCPʱ�õ�
	virtual bool PostRecv(SOCKET hSocket, void *pPerIoData) { return true; };
	virtual bool Disconnect(SOCKET hSocket, bool bForce = false) { return true; };
	virtual int  Recv(SOCKET hSocket, void *pAttachInfo, const char *szData, int nLen) { return 0; };
	virtual int  Send(SOCKET hSocket, const char *szData, int nLen) { return 0; };

	//����socket�ĸ�����Ϣָ��
	virtual bool SetAttachInfo(SOCKET hSocket, void *pAttachInfo) { return true; };

	//TCPClient/UDPSource
	virtual int  Send(const char *szData, int nLen) { return 0; };

	//UDPʱ�õ�
	virtual bool PostRecv(void *pPerIoData) { return true; };
	virtual bool PostSend(void *pPerIoBase) { return true; };
	virtual int  Recv(const char *szData, int nLen, sockaddr *pFrom, int nFromLen) { return 0; };
	virtual int  Send(const char *szData, int nLen, sockaddr *pTo, int nToLen) { return 0; };
	virtual int  Send(const char *szData, int nLen, const char *szServerIP, unsigned short nPort) { return 0; };

public:
	CAppLog		   *m_pAppLog;
	CSocketEvent   *m_pSocketEvent;
	CSocketRunner  *m_pSocketRunner; 

protected:
	//���ջ������Ĵ�С
	//(һ��Ϊ8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536)
	int				m_nRecvBufferSize;

};

};
