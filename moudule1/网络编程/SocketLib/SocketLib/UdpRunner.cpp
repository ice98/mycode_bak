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

	//�˳������߳�
	for (int i=0; i<m_nWorkThreadNum; i++)
	{
		//�����˳���Ϣ
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

	//������ɶ˿ھ��
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,m_nConcurrencyThreadNum);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
#ifdef SHOW_PRINTFINFO
		printf("CUdpRunner::Open Create iocompletionPort failure!\r\n");
#endif
		return false;
	}

    //����Socket�����߳���
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
		//�˳��źŵ���˳��߳�
		if ( -1 == nByteTransferred ) 
		{
			return 1;
		}		
		if ( (NULL==pPerHandle) || (0==nByteTransferred) || 
			 (NULL==pPerIoBase) || (!bSuccess) )
			continue;

		if (pPerIoBase->OperType == RECV_POSTED)
		{//��������

			//��Ϣ����
			PPER_IO_OPERATION_DATA_UDP pPerIoData =(PPER_IO_OPERATION_DATA_UDP)pPerIoBase;
			nRet = pPerHandle->pSocketSource->m_pSocketEvent->OnRecv(pPerHandle->pSocketSource,pPerIoData->szDataBuffer, nByteTransferred, 
				(sockaddr*)(&pPerIoData->addr), pPerIoData->addrlen);
			if (nRet >= 0)  //С��0����ʾ��ЩSocket�ϲ��ٽ�������
				pPerHandle->pSocketSource->PostRecv(pPerIoData);


		}
		else if (pPerIoBase->OperType == SEND_POSTED)
		{//���ݷ������
			pPerHandle->pSocketSource->PostSend(pPerIoBase);
			continue;
		}
	}

	return 0;
}

}