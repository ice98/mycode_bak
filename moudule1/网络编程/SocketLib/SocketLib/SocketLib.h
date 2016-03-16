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

//IPV4��ַ����󳤶�,��192.168.128.210
#define MAX_IPADDR_LENGTH       15  

enum IOCP_OPERTYPE
{
	RECV_POSTED = 1,
	SEND_POSTED = 2,
	ACCEPT_POSTED = 3
};

//���������
class CSocketSource;
typedef struct _PER_HANDLE_DATA
{
	SOCKET		    hSocket;
	CSocketSource  *pSocketSource;  //SocketSourceָ��
	void		   *pAttachInfo;	//�ϲ㸽����Ϣָ��
}PER_HANDLE_DATA,*PPER_HANDLE_DATA;

//��I/O���ݻ����ṹ
typedef struct _PER_IO_OPERATION_BASE
{
	//�ص��ṹ
	OVERLAPPED Overlapped;

	//�������ͱ�ʾ
	IOCP_OPERTYPE OperType;

	//��Դ��
	CRITICAL_SECTION IoLock;

	//���ݻ�����
	WSABUF SocketBuf;
}PER_IO_OPERATION_BASE,*PPER_IO_OPERATION_BASE;

//�������������I/O����
typedef struct _PER_IO_OPERATION_DATA
{
	//�ص��ṹ
	OVERLAPPED Overlapped;

	//�������ͱ�ʾ
	IOCP_OPERTYPE OperType;

	//��Դ��
	//CRITICAL_SECTION IoLock;

	//���ݻ�����
	WSABUF SocketBuf;
	char   *szDataBuffer;
}PER_IO_OPERATION_DATA,*PPER_IO_OPERATION_DATA;

//����Socket�Ự�ṹ
typedef struct _SOCKET_SESSION
{
	PER_HANDLE_DATA		   PerHandle;   //���������
	PER_IO_OPERATION_DATA  PerIoRecv;   //����(I/O)
	PER_IO_OPERATION_BASE  PerIoSend;   //����(I/O)
}SOCKET_SESSION,*PSOCKET_SESSION;


// UDP��ɶ˿ڵ������ṹ

//��I/O���ݻ����ṹ
typedef struct _PER_IO_OPERATION_BASE_UDP
{
	//�ص��ṹ
	OVERLAPPED Overlapped;

	//�������ͱ�ʾ
	IOCP_OPERTYPE OperType;

	//��Դ��
	//CRITICAL_SECTION IoLock;

	//���ݻ�����
	WSABUF SocketBuf;
}PER_IO_OPERATION_BASE_UDP,*PPER_IO_OPERATION_BASE_UDP;

//�������������I/O����
typedef struct _PER_IO_OPERATION_DATA_UDP
{
	//�ص��ṹ
	OVERLAPPED Overlapped;

	//�������ͱ�ʾ
	IOCP_OPERTYPE OperType;

	//��Դ��
	//CRITICAL_SECTION IoLock;

	//���ͷ���ַ
    SOCKADDR_IN		 addr;
    int              addrlen;

	//���ݻ�����
	WSABUF SocketBuf;
	char   *szDataBuffer;
}PER_IO_OPERATION_DATA_UDP,*PPER_IO_OPERATION_DATA_UDP;

};
