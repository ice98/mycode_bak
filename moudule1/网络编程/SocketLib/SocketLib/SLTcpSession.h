//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef SLTCPSESSION_H
#define SLTCPSESSION_H
#pragma once
#include "config.h"
#include "socketsource.h"

namespace SL
{

/*	CTcpSession
	���ܣ����/����Ĵ���
	��Ϣ�Ĺ��:��Ϣ��ʼ��1��2��4���ֽڱ�ʾ��Ϣ��С
		message = messagehead + messagebody
		messagehead = size + command + option
	ע��:
		��������Сһ��Ҫ���ڵ������������Ϣ������
*/
class SOCKETLIB_API CSLTcpSession
{
public:
	CSLTcpSession(void);
	virtual ~CSLTcpSession(void);

	virtual bool Init(CSocketSource *pSource, SOCKET hSocket, uint32 nBufferSize = 1024, uint8 nMsgSizeByte = 2);
	virtual bool Uninit();
	virtual bool Reset();
	virtual bool Clear();

	// ������Ϣ
	virtual int ProcessMessage(const char *szMsg, int nLen);
	// ��������
	virtual int Send(const char *szData, int nLen);
	// ��������
	virtual int OnRecv(const char *szData, int nLen);

private:
	// ȡ����Ϣ��С
	int GetMsgSize(const char *szData, int nLen);
	// ����Buffer
	bool MakeBuffer();
	// ���
	int SplitPacket(const char *szData, int nLen);

protected:
	CSocketSource  *m_pSocketSource;
	SOCKET			m_hSocket;

	char   *m_szBuffer;
	uint32  m_nBufferSize;			//��������С

	//���յ���Ϣ��Size>m_nBufferSizeʱ��Ч�����ϲ㸺����
	//��ʾ����Ҫ�յ���Ϣ����
	uint32  m_nNeedMsgSize;

	uint8   m_nMsgSizeByte;			//��Ϣ��ʽ��Sizeռ�õ��ֽ���(1��2��4)
	uint32  m_nLastMsgLeft;			//����յ���Ϣ�ĳ���

};

};

#endif