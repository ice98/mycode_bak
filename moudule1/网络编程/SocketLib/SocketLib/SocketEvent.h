/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* �ļ����ƣ�SocketEvent.h
* ժ    Ҫ��Socket�¼��ӿ���
* ��ǰ�汾��1.0.0.716
* ��    �ߣ��ű�Ԫ bolidezhang@gmail.com
* ������ڣ�2005/07/16
*
*/

#pragma once
#include "config.h"
#include "SocketLib.h"

namespace SL
{

class SOCKETLIB_API CSocketEvent
{
public:
	CSocketEvent(void);
	virtual ~CSocketEvent(void);

	virtual bool OnAccept(CSocketSource *pSource, SOCKET hClientSocket) { return true; };
	virtual bool OnInitSocket(PPER_HANDLE_DATA pSocketData) { return true; };
	virtual bool OnDisconnect(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo) { return true; };
	virtual int  OnRecv(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen) { return 1; };

	//UDPʱ�õ�
	virtual int  OnRecv(CSocketSource *pSource, const char *szData, int nLen, sockaddr *pFrom, int nFromLen) { return 1; };

};

};
