//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\tcprunner.h"
#include "SocketLib.h"
#include "SocketSource.h"
#include "SocketEvent.h"
namespace SL
{

CTcpRunner::CTcpRunner(void)
{
}

CTcpRunner::~CTcpRunner(void)
{
	Close();
}

bool CTcpRunner::Close()
{
	if (NULL == m_hCompletionPort)
		return false;

	//ʹ�����߳��˳�
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

bool CTcpRunner::Open(unsigned short nConcurrencyThreadNum, unsigned short nWorkThreadNum)
{
	Close();

	if (nWorkThreadNum > 0)
		m_nWorkThreadNum = nWorkThreadNum;

	if ( nConcurrencyThreadNum <= nWorkThreadNum )
		m_nConcurrencyThreadNum = nConcurrencyThreadNum;
	else
		m_nConcurrencyThreadNum = 0;

	//������ɶ˿ھ��
	m_hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,m_nConcurrencyThreadNum);
	if (INVALID_HANDLE_VALUE == m_hCompletionPort)
	{
#ifdef SHOW_PRINTFINFO
		printf("Create iocompletionPort failure!\r\n");
#endif
		return false;
	}

    //����Socket�����߳���
	m_threadGroup.Start(CTcpRunner::Run, this, m_nWorkThreadNum);

	return true;
}

unsigned int WINAPI CTcpRunner::Run(LPVOID lParam)
{
	CTcpRunner *pRun		= (CTcpRunner*)lParam;
	HANDLE hCompletionPort	= pRun->m_hCompletionPort;

	int		nRet;
	DWORD	nByteTransferred;
	PPER_HANDLE_DATA	   pPerHandle;
	PPER_IO_OPERATION_BASE pPerIoBase;

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
		if ( -1==nByteTransferred ) 
		{
			return 1;
		}
		if (NULL == pPerHandle)
			continue;

		//�ͻ����Ѿ��Ͽ����ӻ������ӳ��ִ���
		if ( 0 == nByteTransferred )
		{
			pPerHandle->pSocketSource->Disconnect(pPerHandle->hSocket);
			continue;
		}
		if ( (NULL == pPerIoBase) || (!bSuccess) )  //���������쳣���
			continue;

		if (pPerIoBase->OperType == RECV_POSTED)
		{//���ݽ���

			//��Ϣ����
			PPER_IO_OPERATION_DATA pPerIoData =(PPER_IO_OPERATION_DATA)pPerIoBase;
			//pPerIoData->szDataBuffer[nByteTransferred] = 0;
			//nRet = pPerHandle->pSocketSource->Recv(pPerHandle->hSocket, pPerHandle->pAttachInfo, 
			//	pPerIoData->szDataBuffer, nByteTransferred);
			nRet = pPerHandle->pSocketSource->m_pSocketEvent->OnRecv(pPerHandle->pSocketSource,
				pPerHandle->hSocket, pPerHandle->pAttachInfo, pPerIoData->szDataBuffer, nByteTransferred);
			if (nRet >= 0)  //С��0����ʾ�ڴ�Socket�ϲ��ٽ�������
				pPerHandle->pSocketSource->PostRecv(pPerHandle->hSocket, pPerIoData);

		}
		else if (pPerIoBase->OperType == SEND_POSTED)
		{//���ݷ������
			//pPerHandle->pSocketSource->PostSend(PerHandle->hSocket);
			continue;
		}
	}

	return 0;

}

}
