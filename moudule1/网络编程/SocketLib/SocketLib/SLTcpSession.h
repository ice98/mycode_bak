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
	功能：组包/封包的处理
	消息的规格:消息开始处1、2、4个字节表示消息大小
		message = messagehead + messagebody
		messagehead = size + command + option
	注意:
		缓冲区大小一定要大于等于最大完整消息包长度
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

	// 处理消息
	virtual int ProcessMessage(const char *szMsg, int nLen);
	// 发送数据
	virtual int Send(const char *szData, int nLen);
	// 接收数据
	virtual int OnRecv(const char *szData, int nLen);

private:
	// 取得消息大小
	int GetMsgSize(const char *szData, int nLen);
	// 分配Buffer
	bool MakeBuffer();
	// 拆包
	int SplitPacket(const char *szData, int nLen);

protected:
	CSocketSource  *m_pSocketSource;
	SOCKET			m_hSocket;

	char   *m_szBuffer;
	uint32  m_nBufferSize;			//缓冲区大小

	//当收到消息中Size>m_nBufferSize时有效，由上层负责拆包
	//表示还需要收到消息长度
	uint32  m_nNeedMsgSize;

	uint8   m_nMsgSizeByte;			//消息格式中Size占用的字节数(1、2、4)
	uint32  m_nLastMsgLeft;			//最后收到消息的长度

};

};

#endif