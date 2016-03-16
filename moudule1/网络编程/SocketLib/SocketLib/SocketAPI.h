//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#pragma once
#include "config.h"
namespace SL
{
class SOCKETLIB_API CSocketAPI
{
public:
	CSocketAPI(void);
	~CSocketAPI(void);

	//ͨ��Socketֵȡ��IP��ַ��Port�˿�
	static bool	GetSocketInfo(SOCKET hSocket, LPSTR szIPAddr, USHORT *nPort);

	static bool LoadSocketLib(int version_high, int version_low);
	static bool UnloadSocketLib();

	/*
	��windows2000��ʼ����һ���µ�I/O��������SIO_KEEPALIVE_VALS����ͨ��WSAIoctl()���
	�����׽��������Ƿ����ñ�����ơ�KeepAliveTime��KeepAliveInterval��
	Argument structure for SIO_KEEPALIVE_VALS
	struct tcp_keepalive {
		u_long  onoff;
		u_long  keepalivetime;
		u_long  keepaliveinterval;
	};
	*/
	static BOOL GetSocketKeepAlive(SOCKET hSocket);
	static bool SetSocketKeepAlive(SOCKET hSocket, BOOL bKeepAlive);
	static bool SetSocketKeepAliveValue(SOCKET hSocket, unsigned long nKeepAliveTime=7200000, unsigned long nKeepAliveInterval=1000);

	//SO_LINGER
	static bool SetSocketLinger(SOCKET hSocket, unsigned long nOnoff, unsigned long nLinger);
};

};
