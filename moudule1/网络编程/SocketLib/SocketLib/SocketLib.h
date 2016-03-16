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

#define DEFAULT_BUFFER_SIZE       8192
#define RECV_BUFFER_SIZE          8192
#define SEND_BUFFER_SIZE          8192
#define MAX_SOCKET_CONNECTNUM   500000
#define MAX_SOCKET_FREENUM      500000
#define MAX_LOGBUFFER_LENGTH      1024

//IPV4地址的最大长度,如192.168.128.210
#define MAX_IPADDR_LENGTH       15  

enum IOCP_OPERTYPE
{
	RECV_POSTED = 1,
	SEND_POSTED = 2,
	ACCEPT_POSTED = 3
};

//单句柄数据
class CSocketSource;
typedef struct _PER_HANDLE_DATA
{
	SOCKET		    hSocket;
	CSocketSource  *pSocketSource;  //SocketSource指针
	void		   *pAttachInfo;	//上层附加信息指针
}PER_HANDLE_DATA,*PPER_HANDLE_DATA;

//单I/O数据基础结构
typedef struct _PER_IO_OPERATION_BASE
{
	//重叠结构
	OVERLAPPED Overlapped;

	//操作类型表示
	IOCP_OPERTYPE OperType;

	//资源锁
	CRITICAL_SECTION IoLock;

	//数据缓冲区
	WSABUF SocketBuf;
}PER_IO_OPERATION_BASE,*PPER_IO_OPERATION_BASE;

//带缓冲区单句柄I/O数据
typedef struct _PER_IO_OPERATION_DATA
{
	//重叠结构
	OVERLAPPED Overlapped;

	//操作类型表示
	IOCP_OPERTYPE OperType;

	//资源锁
	//CRITICAL_SECTION IoLock;

	//数据缓冲区
	WSABUF SocketBuf;
	char   *szDataBuffer;
}PER_IO_OPERATION_DATA,*PPER_IO_OPERATION_DATA;

//单个Socket会话结构
typedef struct _SOCKET_SESSION
{
	PER_HANDLE_DATA		   PerHandle;   //单句柄数据
	PER_IO_OPERATION_DATA  PerIoRecv;   //接收(I/O)
	PER_IO_OPERATION_BASE  PerIoSend;   //发送(I/O)
}SOCKET_SESSION,*PSOCKET_SESSION;


// UDP完成端口的两个结构

//单I/O数据基础结构
typedef struct _PER_IO_OPERATION_BASE_UDP
{
	//重叠结构
	OVERLAPPED Overlapped;

	//操作类型表示
	IOCP_OPERTYPE OperType;

	//资源锁
	//CRITICAL_SECTION IoLock;

	//数据缓冲区
	WSABUF SocketBuf;
}PER_IO_OPERATION_BASE_UDP,*PPER_IO_OPERATION_BASE_UDP;

//带缓冲区单句柄I/O数据
typedef struct _PER_IO_OPERATION_DATA_UDP
{
	//重叠结构
	OVERLAPPED Overlapped;

	//操作类型表示
	IOCP_OPERTYPE OperType;

	//资源锁
	//CRITICAL_SECTION IoLock;

	//发送方地址
    SOCKADDR_IN		 addr;
    int              addrlen;

	//数据缓冲区
	WSABUF SocketBuf;
	char   *szDataBuffer;
}PER_IO_OPERATION_DATA_UDP,*PPER_IO_OPERATION_DATA_UDP;

};
