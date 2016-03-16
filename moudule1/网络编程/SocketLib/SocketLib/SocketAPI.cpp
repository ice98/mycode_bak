//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "socketapi.h"
#include <mstcpip.h>
namespace SL
{

CSocketAPI::CSocketAPI(void)
{
}

CSocketAPI::~CSocketAPI(void)
{
}

bool CSocketAPI::LoadSocketLib(int version_high, int version_low)
{
	WSADATA wsaData;
    WORD version_requested = MAKEWORD(version_high, version_low);
	int nError = WSAStartup(version_requested,&wsaData);
	if (nError != 0)
	{
		return false;
	}	
	return true;
}

bool CSocketAPI::UnloadSocketLib()
{
	WSACleanup();
	return true; 
}

//取得客户端信息
bool CSocketAPI::GetSocketInfo(SOCKET hSocket, LPSTR szIPAddr, USHORT *nPort)
{
	sockaddr soad;
	sockaddr_in in;
	int nLen = sizeof(soad);
	if (getpeername(hSocket,&soad,&nLen) == SOCKET_ERROR)
	{
		return false;
	}
	memcpy(&in,&soad,sizeof(sockaddr));
	strcpy(szIPAddr,inet_ntoa(in.sin_addr));
	*nPort = ntohs(in.sin_port);
	return true;
}

BOOL CSocketAPI::GetSocketKeepAlive(SOCKET hSocket)
{
	BOOL bRet = 0;
	int  nOptLen = sizeof(BOOL);
	if (SOCKET_ERROR==getsockopt(hSocket,SOL_SOCKET,SO_KEEPALIVE,(char*)&bRet,&nOptLen))
		return 0;
	return bRet;
}

bool CSocketAPI::SetSocketKeepAlive(SOCKET hSocket, BOOL bKeepAlive)
{
	if (SOCKET_ERROR==setsockopt(hSocket,SOL_SOCKET,SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive)))
		return false;
	return true;
}

bool CSocketAPI::SetSocketKeepAliveValue(SOCKET hSocket, unsigned long nKeepAliveTime, unsigned long nKeepAliveInterval)
{
	tcp_keepalive inKeepAlive	= {0};
    tcp_keepalive outKeepAlive	= {0};
	unsigned long nInLen	= sizeof(tcp_keepalive);
	unsigned long nOutLen	= sizeof(tcp_keepalive);
	unsigned long nBytesReturn = 0;
     
    //设置socket的keepalivetime,keepaliveinterval
	inKeepAlive.onoff = 1;
    inKeepAlive.keepalivetime = nKeepAliveTime;
    inKeepAlive.keepaliveinterval = nKeepAliveInterval;
                   
    //为选定的SOCKET设置Keep Alive，成功后SOCKET可通过KeepAlive自动检测连接是否断开 
    if ( SOCKET_ERROR == WSAIoctl(hSocket, SIO_KEEPALIVE_VALS,(LPVOID)&inKeepAlive, nInLen,(LPVOID)&outKeepAlive, nOutLen, &nBytesReturn, NULL, NULL))
		return false;
	return true;
}

bool CSocketAPI::SetSocketLinger(SOCKET hSocket, unsigned long nOnoff, unsigned long nLinger)
{
	linger l;
	l.l_onoff  = nOnoff;
	l.l_linger = nLinger;
	if (SOCKET_ERROR==::setsockopt(hSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&l), sizeof(l)) )
		return false;
	return true;
}

}