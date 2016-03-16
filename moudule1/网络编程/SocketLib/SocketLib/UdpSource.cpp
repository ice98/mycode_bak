//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include "udpsource.h"
#include "SocketLib.h"
#include "SocketEvent.h"
#include <assert.h>
#include "sync/guard.h"

namespace SL
{

CUdpSource::CUdpSource()
	: m_hSocket(NULL)
	, m_nMaxFreeRecvPoolNum(16)
	, m_nMaxFreeSendPoolNum(16)
{
	m_perHandle.pSocketSource = this;
}

CUdpSource::CUdpSource(int nMaxFreeRecvPoolNum, int nMaxFreeSendPoolNum)
	: m_hSocket(NULL)
{
	if (nMaxFreeRecvPoolNum<1)
		m_nMaxFreeRecvPoolNum = 16;
	else
		nMaxFreeRecvPoolNum = nMaxFreeRecvPoolNum;
	if (nMaxFreeSendPoolNum<1)
		m_nMaxFreeSendPoolNum = 16;
	else
		m_nMaxFreeSendPoolNum = nMaxFreeRecvPoolNum;
	m_perHandle.pSocketSource = this;
}

CUdpSource::~CUdpSource(void)
{
	Close();
}

inline bool CUdpSource::Close()
{
	{
		SYNC::CGuard<SYNC::CThreadMutex> guard(m_lock);
		if (m_hSocket != NULL)
		{
			shutdown(m_hSocket,0);
			closesocket(m_hSocket);
			m_hSocket = NULL;
		}
	}

	//清除空闲池
	{
		SYNC::CGuard<SYNC::CThreadMutex> guard(m_lockRecv);
		std::list<PPER_IO_OPERATION_DATA_UDP>::iterator itRecv;
		for (itRecv = m_lstFreeRecvPool.begin(); itRecv != m_lstFreeRecvPool.end(); itRecv++)
		{
			HeapFree(GetProcessHeap(),0,*itRecv);
		}
		m_lstFreeRecvPool.clear();
	}
	{
		SYNC::CGuard<SYNC::CThreadMutex> guard(m_lockSend);
		std::list<PPER_IO_OPERATION_BASE_UDP>::iterator itSend;
		for (itSend = m_lstFreeSendPool.begin(); itSend != m_lstFreeSendPool.end(); itSend++)
		{
			HeapFree(GetProcessHeap(),0,*itSend);
		}
		m_lstFreeSendPool.clear();
	}

	return true;
}

bool CUdpSource::Open(const char *szServerName, unsigned short nServerPort, int nLocalPort)
{
	Close();
	SYNC::CGuard<SYNC::CThreadMutex> guard(m_lock);

	//创建Socket
	m_hSocket = WSASocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET==m_hSocket)
	{
#ifdef SHOW_PRINTFINFO
		printf("Create socket failure!\r\n");
#endif
		return false;
	}

	//主机名解释
	memset(&m_ServerAddr,0,sizeof(SOCKADDR_IN));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = htons(nServerPort);
	unsigned long   nAddr;
	nAddr = inet_addr(szServerName);
	if (nAddr == INADDR_NONE)
	{
		struct hostent *hp = NULL;
		hp = gethostbyname(szServerName);
		if (hp != NULL)
			memcpy(&(m_ServerAddr.sin_addr),hp->h_addr,hp->h_length);
		else
			return false;
	}
	else
  		m_ServerAddr.sin_addr.s_addr = nAddr;

	if ( (nLocalPort>0) && (nLocalPort<65535) )
	{
		SOCKADDR_IN LoacalAddr;
		memset(&LoacalAddr,0,sizeof(LoacalAddr));
		LoacalAddr.sin_family = AF_INET;
		LoacalAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		LoacalAddr.sin_port = htons(nLocalPort);
		if (bind(m_hSocket,(SOCKADDR*)&LoacalAddr,sizeof(LoacalAddr)) == SOCKET_ERROR)
		{
#ifdef SHOW_PRINTFINFO
			printf(TEXT("UDP bind port socket failure!\r\n"));
#else
	#ifdef SOCKETLIB_WRITE_LOG
			m_pAppLog->WriteLog(TEXT("UDP bind port socket failure!"), TEXT(__FILE__), __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif
			return false;
		}
	}
	else
	{
		int nRet = connect(m_hSocket,(SOCKADDR*)&m_ServerAddr,sizeof(m_ServerAddr));
		if (SOCKET_ERROR == nRet)
		{
#ifdef SHOW_PRINTFINFO
			printf("UdpSource port invalid!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
			m_pAppLog->WriteLog(TEXT("UdpSource port invalid!"), TEXT(__FILE__), __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif
			return false;
		}
	}

	//关联Socket到完成端口，句柄数据在此时被绑定到完成端口
	m_perHandle.hSocket = m_hSocket;
	m_pSocketRunner->AssociateSocket(m_hSocket, &m_perHandle);
	for (int i=0; i<m_pSocketRunner->GetWorkThreadNum(); i++)
	{
		PPER_IO_OPERATION_DATA_UDP p = GetRecvObj();
		if (NULL == p)
			return false;
		PostRecv(p);
	}

#ifdef SHOW_PRINTFINFO
	printf("UdpSource::Open success, socket:%ld localport:%d!\r\n", m_hSocket, nLocalPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
	SL_TCHAR szLog[MAX_LOGBUFFER_LENGTH]={0};
	sprintf(szLog, "UDPSource::Open success, socket:%ld localport:%ld!", m_hSocket, nLocalPort);
	m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	return true;
}

bool CUdpSource::Open(unsigned short nLocalPort)
{
	Close();
	SYNC::CGuard<SYNC::CThreadMutex> guard(m_lock);

	//创建Socket
	m_hSocket = WSASocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET==m_hSocket)
	{
#ifdef SHOW_PRINTFINFO
		printf("Create socket failure!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("Create socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
	#endif
#endif
		return false;
	}

	SOCKADDR_IN LoacalAddr;
	LoacalAddr.sin_family = AF_INET;
	LoacalAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	LoacalAddr.sin_port = htons(nLocalPort);
	if (bind(m_hSocket,(SOCKADDR*)&LoacalAddr,sizeof(LoacalAddr)) == SOCKET_ERROR)
	{
#ifdef SHOW_PRINTFINFO
		printf("UDP bind port socket failure!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("UDP bind port socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif
		return false;
	}

	//关联Socket到完成端口，句柄数据在此时被绑定到完成端口
	m_perHandle.hSocket = m_hSocket;
	m_pSocketRunner->AssociateSocket(m_hSocket, &m_perHandle);
	for (int i=0; i<m_pSocketRunner->GetWorkThreadNum(); i++)
	{
		PPER_IO_OPERATION_DATA_UDP p = GetRecvObj();
		if (NULL == p)
			return false;
		PostRecv(p);
	}

#ifdef SHOW_PRINTFINFO
	printf("UDPSource::Open success, socket:%ld, localport:%ld!\r\n", m_hSocket, nLocalPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
	char szLog[MAX_LOGBUFFER_LENGTH]={0};
	sprintf(szLog, "UDPSource::Open success, socket:%ld, localport:%ld!", m_hSocket, nLocalPort);
	m_pAppLog->WriteLog("UDPSource::Open success!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	return true;
}

void CUdpSource::SetBufferSize(int nRecvBufferSize, int nSendBufferSize)
{
	if (nRecvBufferSize > 0)
		m_nRecvBufferSize = nRecvBufferSize;
	return;
}

void CUdpSource::SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner)
{
	assert(pAppLog != NULL);
	assert(pSocketEvent != NULL);
	assert(pSocketRunner != NULL);
	m_pAppLog = pAppLog;
	m_pSocketEvent = pSocketEvent;
	m_pSocketRunner = pSocketRunner;
	return;
}

inline bool CUdpSource::PostRecv(void *p)
{
	if ( p == NULL )
		return false;
	PPER_IO_OPERATION_DATA_UDP pPerIoData = (PPER_IO_OPERATION_DATA_UDP)p;

	//重置IO操作数据
	DWORD nFlag = 0;
	DWORD nRecvByte = 0;
	ZeroMemory(&(pPerIoData->Overlapped),sizeof(OVERLAPPED));
	pPerIoData->SocketBuf.buf = pPerIoData->szDataBuffer;
	pPerIoData->SocketBuf.len = m_nRecvBufferSize;
	pPerIoData->OperType = RECV_POSTED;

	//提交PostRecv请求
	int nRet = WSARecvFrom(m_hSocket,&(pPerIoData->SocketBuf), 1, &nRecvByte, &nFlag, 
		(sockaddr*)&(pPerIoData->addr), &(pPerIoData->addrlen), &(pPerIoData->Overlapped), NULL);

	if ( SOCKET_ERROR == nRet ) 
	{
		int nErrorID = WSAGetLastError();
		if (nErrorID != WSA_IO_PENDING)
		{//接收失败,最有可能是Socket已断开,所以按断开处理
			FreeRecvObj(pPerIoData);
#ifdef SHOW_PRINTFINFO
			printf("UDPSource::PostRecv: error:%d!\r\n", nErrorID);
#else
	#ifdef SOCKETLIB_WRITE_LOG
			char szLog[MAX_LOGBUFFER_LENGTH] = {0};
			sprintf(szLog,"PostRead error:%ld!", nErrorID);
			m_pAppLog->WriteLog(szLog,__FILE__,__LINE__,CAppLog::LOG_LEVEL_5);
	#endif
#endif
			return false;
		}
	}
	return true;
}

inline bool CUdpSource::PostSend(void *p)
{
	PPER_IO_OPERATION_BASE_UDP pPerIoBase = (PPER_IO_OPERATION_BASE_UDP)p;
	FreeSendObj(pPerIoBase);
	return true;
}

inline int CUdpSource::Recv(const char *szData, int nLen, sockaddr *pFrom, int nFromLen)
{
	return m_pSocketEvent->OnRecv(this, szData, nLen, pFrom, nFromLen);
}

int CUdpSource::Send(const char *szData, int nLen, sockaddr *pTo, int nToLen)
{
	if ( (szData==NULL) || (nLen<=0) )
	{
		return -1;
	}
	PPER_IO_OPERATION_BASE_UDP pPerIoBase = GetSendObj();
	if (NULL == pPerIoBase)
		return -1;

	DWORD nFlag = 0;
	DWORD nSendByte = 0;
	ZeroMemory(&(pPerIoBase->Overlapped),sizeof(OVERLAPPED));
	pPerIoBase->SocketBuf.buf = (char*)szData;
	pPerIoBase->SocketBuf.len = nLen;
	pPerIoBase->OperType = SEND_POSTED;
	int nRet = WSASendTo(m_hSocket, &(pPerIoBase->SocketBuf), 1, &nSendByte, nFlag, 
		pTo, nToLen, &(pPerIoBase->Overlapped), NULL);

	if ( nRet==SOCKET_ERROR )
	{
		int nErrorID = WSAGetLastError();
		if (nErrorID != WSA_IO_PENDING)
		{//发送失败,最有可能是Socket已断开,所以按断开处理
			FreeSendObj(pPerIoBase);
#ifdef SHOW_PRINTFINFO
			printf("UDPSource::Send error:%d!\r\n",nErrorID);
#else
	#ifdef SOCKETLIB_WRITE_LOG
			SL_TCHAR szLog[MAX_LOGBUFFER_LENGTH] = {0};
			_stprintf(szLog,TEXT("UDPSource::Send error:%d!"), nErrorID);
			m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif
			return -1;
		}
	}
	return nSendByte;

}

inline int CUdpSource::Send(const char *szData, int nLen, const char *szIP, unsigned short nPort)
{
	sockaddr_in ServerAddr;
	ServerAddr.sin_family	= AF_INET;
	ServerAddr.sin_port		= htons(nPort);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(szIP);
	return Send(szData, nLen, (sockaddr*)&ServerAddr, sizeof(sockaddr_in));
}

inline int CUdpSource::Send(const char *szData, int nLen)
{
	return Send(szData, nLen, (sockaddr*)&m_ServerAddr, sizeof(sockaddr_in));
}

PPER_IO_OPERATION_DATA_UDP CUdpSource::GetRecvObj()
{	
	m_lockRecv.Lock();
	PPER_IO_OPERATION_DATA_UDP p = NULL;
	if (m_lstFreeRecvPool.empty())
	{
		p = (PPER_IO_OPERATION_DATA_UDP)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, (sizeof(PER_IO_OPERATION_DATA_UDP)+m_nRecvBufferSize) );
		if (NULL == p)
		{
#ifdef SHOW_PRINTFINFO
			printf("UDPSource::GetRecvObj: HeapAlloc memory error!\r\n");
#endif
			m_lockRecv.UnLock();
			return NULL;
		}
		p->addrlen		 = sizeof(sockaddr_in);
		p->szDataBuffer  = ((char*)p+sizeof(PER_IO_OPERATION_DATA_UDP));
	}
	else
	{
		p = m_lstFreeRecvPool.front();
		m_lstFreeRecvPool.pop_front();
	}
	m_lockRecv.UnLock();
	return p;
}

void CUdpSource::FreeRecvObj(PPER_IO_OPERATION_DATA_UDP p)
{
	if ( NULL == p)
		return;
	m_lockRecv.Lock();
	if (m_lstFreeRecvPool.size() >= m_nMaxFreeRecvPoolNum)
		HeapFree(GetProcessHeap(),0,(void*)p);
	else
		m_lstFreeRecvPool.push_back(p);
	m_lockRecv.UnLock();

}

PPER_IO_OPERATION_BASE_UDP CUdpSource::GetSendObj()
{	
	m_lockSend.Lock();
	PPER_IO_OPERATION_BASE_UDP p = NULL;
	if (m_lstFreeSendPool.empty())
	{
		p = (PPER_IO_OPERATION_BASE_UDP)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, (sizeof(PER_IO_OPERATION_BASE_UDP)) );
		if (NULL == p)
		{
#ifdef SHOW_PRINTFINFO
			printf("UDPSource::GetSendObj: HeapAlloc memory error!\r\n");
#endif
			m_lockSend.UnLock();
			return NULL;
		}
	}
	else
	{
		p = m_lstFreeSendPool.front();
		m_lstFreeSendPool.pop_front();
	}
	m_lockSend.UnLock();
	return p;
}

void CUdpSource::FreeSendObj(PPER_IO_OPERATION_BASE_UDP p)
{
	if ( NULL == p)
		return;

	m_lockSend.Lock();
	if (m_lstFreeSendPool.size() >= m_nMaxFreeSendPoolNum)
		HeapFree(GetProcessHeap(),0,(void*)p);
	else
		m_lstFreeSendPool.push_back(p);
	m_lockSend.Lock();

}

}