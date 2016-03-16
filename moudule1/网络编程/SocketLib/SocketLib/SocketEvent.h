/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* 文件名称：SocketEvent.h
* 摘    要：Socket事件接口类
* 当前版本：1.0.0.716
* 作    者：张保元 bolidezhang@gmail.com
* 完成日期：2005/07/16
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

	//UDP时用到
	virtual int  OnRecv(CSocketSource *pSource, const char *szData, int nLen, sockaddr *pFrom, int nFromLen) { return 1; };

};

};
