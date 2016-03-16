/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* �ļ����ƣ�SocketRunner.h
* ժ    Ҫ��Socket��������ӿ���
* ��    �ߣ��ű�Ԫ bolidezhang@gmail.com
* ������ڣ�2005/07/16
*
*/

#pragma once
#include "Config.h"
#include "sync/ThreadGroup.h"

namespace SL
{

// Socket�¼��ķ�����
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

	//��ɶ˿ھ��
	HANDLE	        m_hCompletionPort; 

	//�����߳���
	short			m_nConcurrencyThreadNum;
	//�����߳���
	short			m_nWorkThreadNum;

	//�����߳���
	SYNC::CThreadGroup m_threadGroup;

};

};
