//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\socketrunner.h"
#include "SocketLib.h"
namespace SL
{

CSocketRunner::CSocketRunner(void)
	: m_hCompletionPort(NULL)
	, m_nConcurrencyThreadNum(0)
{
	//默认Socket线程数为CPU处理器数量*2+2
	SYSTEM_INFO sys_Info;
	GetSystemInfo(&sys_Info);
	m_nWorkThreadNum = sys_Info.dwNumberOfProcessors * 2 + 2;
}

CSocketRunner::~CSocketRunner(void)
{
}

inline bool CSocketRunner::AssociateSocket(SOCKET hSocket, void *pPerHandle)
{
	//将关联Socket到完成端口，句柄数据在此时被绑定到完成端口
	if ( CreateIoCompletionPort((HANDLE)hSocket, m_hCompletionPort,(ULONG_PTR)pPerHandle, m_nConcurrencyThreadNum) == NULL )
		return false;

	return true;
}

inline short CSocketRunner::GetWorkThreadNum()
{
	return m_nWorkThreadNum;
}

}
