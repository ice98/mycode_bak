/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* 文件名称：SocketRunner.h
* 摘    要：Socket分派引擎接口类
* 作    者：张保元 bolidezhang@gmail.com
* 完成日期：2005/07/16
*
*/

#pragma once
#include "Config.h"
#include "sync/ThreadGroup.h"

namespace SL
{

// Socket事件的分派类
class SOCKETLIB_API CSocketRunner
{
public:
	CSocketRunner(void);
	virtual ~CSocketRunner(void);

	virtual bool  Open(unsigned short nConcurrencyThreadNum = 0, unsigned short nWorkThreadNum = 4 ) = 0;
	virtual bool  Close() = 0;
	virtual bool  AssociateSocket(SOCKET hSocket, void *pPerHandle);
	virtual short GetWorkThreadNum();

protected:

	//完成端口句柄
	HANDLE	        m_hCompletionPort; 

	//并行线程数
	short			m_nConcurrencyThreadNum;
	//工作线程数
	short			m_nWorkThreadNum;

	//工作线程组
	SYNC::CThreadGroup m_threadGroup;

};

};
