//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\udprunner.h"
#include "SocketLib.h"
#include "SocketSource.h"
#include "SocketEvent.h"
namespace SL
{

CUdpRunner::CUdpRunner(void)
{
}

CUdpRunner::~CUdpRunner(void)
{
	Close();
}

bool CUdpRunner::Close()
{
	if (NULL == m_hCompletionPort)
		return false;

	//退出工作线程
	for (int i=0; i<m_nWorkThreadNum; i++)
	{
		//发出退出消息
		::PostQueuedCompletionStatus(m_hCompletionPort,-1,-1,NULL);
	}
	m_threadGroup.Stop(500);
	m_threadGroup.Kill();

	CloseHandle(m_hCompletionPort);
	m_hCompletionPort = NULL;

	return true;
}

bool CUdpRunner::Open(unsigned short nConcurrencyThreadNum, unsigned short nWorkThreadNum)
{
	Close();

	if (nWorkThreadNum > 0)
		m_nWorkThreadNum = nWorkThreadNum;

	if ( nConcurrencyThreadNum <= nWorkThreadNum )
		m_nConcurrencyThreadNum = nConcurrencyThreadNum;
	else
		m_nConcurrencyThreadNum = 0;

	//创建完成端口句柄
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,m_nConcurrencyThreadNum);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
#ifdef SHOW_PRINTFINFO
		printf("CUdpRunner::Open Create iocompletionPort failure!\r\n");
#endif
		return false;
	}

    //启动Socket工作线程组
	m_threadGroup.Start(CUdpRunner::Run, this, m_nWorkThreadNum);

	return true;
}

unsigned int WINAPI CUdpRunner::Run(LPVOID lParam)
{
	CUdpRunner *pRun		= (CUdpRunner*)lParam;
	HANDLE hCompletionPort	= pRun->m_hCompletionPort;

	int		nRet;
	DWORD	nByteTransferred;
	PPER_HANDLE_DATA			pPerHandle;
	PPER_IO_OPERATION_BASE_UDP  pPerIoBase;

	for (;;)
	{
		nByteTransferred = 0;
		pPerHandle = NULL;
		pPerIoBase = NULL;
		BOOL bSuccess = ::GetQueuedCompletionStatus(hCompletionPort,
												&nByteTransferred,
												(LPDWORD)&pPerHandle,
												(LPOVERLAPPED*)&pPerIoBase,
												INFINITE);
		//退出信号到达，退出线程
		if ( -1 == nByteTransferred ) 
		{
			return 1;
		}		
		if ( (NULL==pPerHandle) || (0==nByteTransferred) || 
			 (NULL==pPerIoBase) || (!bSuccess) )
			continue;

		if (pPerIoBase->OperType == RECV_POSTED)
		{//接收数据

			//消息处理
			PPER_IO_OPERATION_DATA_UDP pPerIoData =(PPER_IO_OPERATION_DATA_UDP)pPerIoBase;
			nRet = pPerHandle->pSocketSource->m_pSocketEvent->OnRecv(pPerHandle->pSocketSource,pPerIoData->szDataBuffer, nByteTransferred, 
				(sockaddr*)(&pPerIoData->addr), pPerIoData->addrlen);
			if (nRet >= 0)  //小于0，表示在些Socket上不再接收数据
				pPerHandle->pSocketSource->PostRecv(pPerIoData);


		}
		else if (pPerIoBase->OperType == SEND_POSTED)
		{//数据发送完成
			pPerHandle->pSocketSource->PostSend(pPerIoBase);
			continue;
		}
	}

	return 0;
}

}