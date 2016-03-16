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

	//通过Socket值取得IP地址和Port端口
	static bool	GetSocketInfo(SOCKET hSocket, LPSTR szIPAddr, USHORT *nPort);

	static bool LoadSocketLib(int version_high, int version_low);
	static bool UnloadSocketLib();

	/*
	从windows2000开始引入一个新的I/O控制命令SIO_KEEPALIVE_VALS，可通过WSAIoctl()针对
	单个套接字设置是否启用保活机制、KeepAliveTime、KeepAliveInterval。
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
