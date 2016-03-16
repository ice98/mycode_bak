//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\tcpclient.h"
#include "SocketEvent.h"
#include <assert.h>
#include "sync/guard.h"
namespace SL
{

CTcpClient::CTcpClient(void)
	: CSocketSource()
	, m_bIsConnected(false)
	, m_hSocket(INVALID_SOCKET)
	, m_strServerName("")
	, m_nServerPort(0)
	, m_nLocalPort(0)
{
	ZeroMemory(&m_SocketSession, sizeof(SOCKET_SESSION));
	m_SocketSession.PerHandle.pSocketSource = this;
	InitializeCriticalSection(&(m_SocketSession.PerIoSend.IoLock));
	m_SocketSession.PerIoRecv.szDataBuffer = NULL;
	m_SocketSession.PerHandle.pAttachInfo  = NULL;
}

CTcpClient::~CTcpClient(void)
{
	Close();
	HeapFree(GetProcessHeap(), 0, m_SocketSession.PerIoRecv.szDataBuffer);
	DeleteCriticalSection(&(m_SocketSession.PerIoSend.IoLock));
}

inline bool CTcpClient::IsConnected() const 
{
	return m_bIsConnected;
}

inline bool CTcpClient::Disconnect(SOCKET hSocket, bool bForce)
{
	m_bIsConnected = false;
#ifdef SHOW_PRINTFINFO
	printf("tcpClient disconnect, server:%s port:%d!\r\n", m_strServerName.c_str(),m_nServerPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
	char szLog[MAX_LOGBUFFER_LENGTH] = {0};
	sprintf(szLog, "tcpClient disconnect, server:%s port:%d!\r\n", m_strServerName.c_str(),m_nServerPort);
	m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
	if (!bForce)
		m_pSocketEvent->OnDisconnect(this, hSocket, m_SocketSession.PerHandle.pAttachInfo);
	m_SocketSession.PerHandle.pAttachInfo = NULL;
	return true;
}

inline bool CTcpClient::PostRecv(SOCKET hSocket, void *p)
{
	//if ( (hSocket == INVALID_SOCKET) || (p == NULL) )
	//{
	//	return false;
	//}
	PPER_IO_OPERATION_DATA pPerIoData = (PPER_IO_OPERATION_DATA)p;

	//重置IO操作数据
	DWORD nFlag = 0;
	DWORD nRecvByte = 0;
	ZeroMemory(&(pPerIoData->Overlapped), sizeof(OVERLAPPED));
	ZeroMemory(pPerIoData->szDataBuffer, m_nRecvBufferSize);
	pPerIoData->SocketBuf.buf = pPerIoData->szDataBuffer;
	pPerIoData->SocketBuf.len = m_nRecvBufferSize;
	pPerIoData->OperType	  = RECV_POSTED;

	//提交PostRecv请求
	int nRet = WSARecv(hSocket, &(pPerIoData->SocketBuf), 1,
		&nRecvByte, &nFlag, &(pPerIoData->Overlapped), NULL);

	if (SOCKET_ERROR == nRet) 
	{
		int nErrorID = WSAGetLastError();
		if (nErrorID != WSA_IO_PENDING)
		{//接收失败,最有可能是Socket已断开,所以按断开处理
			Disconnect(hSocket);
#ifdef SHOW_PRINTFINFO
			printf("CTcpClient::PostRecv: error:%d!\r\n", nErrorID);
#endif
			return false;
		}
	}
	return true;
}

inline int CTcpClient::Recv(SOCKET hSocket, void *pAttachInfo, const char *szData, int nLen)
{
	return m_pSocketEvent->OnRecv(this, hSocket, pAttachInfo, szData, nLen);
}

int CTcpClient::Send(const char *szData, int nLen, bool bConnected)
{
	//if ( (NULL == szData) || (nLen<=0) )
	//{
	//	return -1;
	//}
	if (bConnected)
	{
		if (!m_bIsConnected)
			Open(m_strServerName.c_str(), m_nServerPort, m_nLocalPort);
	}
	if (!m_bIsConnected)
		return -1;
	EnterCriticalSection(&(m_SocketSession.PerIoSend.IoLock));

	//重置缓冲区
	DWORD nFlag = 0;
	DWORD nSendByte = 0;
	ZeroMemory(&(m_SocketSession.PerIoSend.Overlapped),sizeof(OVERLAPPED));
	m_SocketSession.PerIoSend.SocketBuf.buf = (char*)szData;
	m_SocketSession.PerIoSend.SocketBuf.len = nLen;
	m_SocketSession.PerIoSend.OperType = SEND_POSTED;
	int nRet = WSASend(m_hSocket,&(m_SocketSession.PerIoSend.SocketBuf),1,
		&nSendByte,nFlag,&(m_SocketSession.PerIoSend.Overlapped),NULL);

	LeaveCriticalSection(&(m_SocketSession.PerIoSend.IoLock));
	if (SOCKET_ERROR == nRet)
	{
		int nErrorID = WSAGetLastError();
		if (nErrorID != WSA_IO_PENDING)
		{//发送失败,最有可能是Socket已断开,所以按断开处理
			Disconnect(m_hSocket);
#ifdef SHOW_PRINTFINFO
			printf("TcpClient::Send error:%d, Socket:%ld!\r\n",nErrorID, m_hSocket);
#else
	#ifdef SOCKETLIB_WRITE_LOG
			char szLog[MAX_LOGBUFFER_LENGTH] = {0};
			sprintf(szLog, "TcpClient::Send error:%d, Socket:%ld!",nErrorID, m_hSocket);
			m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif
			return -1;
		}
	}
	return nSendByte;
}

void CTcpClient::SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner)
{
	assert(pAppLog != NULL);
	assert(pSocketEvent != NULL);
	assert(pSocketRunner != NULL);
	m_pAppLog		= pAppLog;
	m_pSocketEvent	= pSocketEvent;
	m_pSocketRunner = pSocketRunner;
	return;
}

inline void CTcpClient::SetBufferSize(int nRecvBufferSize, int nSendBufferSize)
{
	if (nRecvBufferSize > 0)
	{
		m_nRecvBufferSize = nRecvBufferSize;
	}
	return;
}

bool CTcpClient::Open(const char *szServerName, unsigned short nServerPort, int nLocalPort)
{
	Close();
	SYNC::CGuard<SYNC::CThreadMutex> guard(m_lock);

	//创建Socket
	m_hSocket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET==m_hSocket)
	{
#ifdef SHOW_PRINTFINFO
		printf("Create socket failure!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("TcpClient create socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
		return false;
	}
	if ( (nLocalPort>0) && (nLocalPort<65535) )
	{
		SOCKADDR_IN LoacalAddr;

		LoacalAddr.sin_family = AF_INET;
		LoacalAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		LoacalAddr.sin_port = htons(nLocalPort);
		if (bind(m_hSocket,(SOCKADDR*)&LoacalAddr,sizeof(LoacalAddr)) == SOCKET_ERROR)
		{
#ifdef SHOW_PRINTFINFO
			printf("TcpClient bind port socket failure!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
			m_pAppLog->WriteLog("TcpClient bind port socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
			return false;
		}
	}

	m_nServerPort	= nServerPort;
	m_nLocalPort	= nLocalPort;
	//可能Send函数引起的重连，这时
	if (m_strServerName.c_str() != szServerName) 
		m_strServerName	= szServerName;

	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr,0,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(nServerPort);
	unsigned long   nAddr;
	nAddr = inet_addr(szServerName);
	if (nAddr == INADDR_NONE)
	{
		struct hostent *hp = NULL;
		hp = gethostbyname(szServerName);
		if (hp != NULL)
			memcpy(&(ServerAddr.sin_addr),hp->h_addr,hp->h_length);
		else
			return false;
	}
	else
  		ServerAddr.sin_addr.s_addr = nAddr;
	int nRet = connect(m_hSocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr));
	if (SOCKET_ERROR == nRet)
	{
#ifdef SHOW_PRINTFINFO
		printf("TcpClient::Open connect error!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("TcpClient::Open connect error!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
		return false;
	}
	if (m_SocketSession.PerIoRecv.szDataBuffer != NULL)
	{
		m_SocketSession.PerIoRecv.szDataBuffer = (char*)HeapReAlloc(
			GetProcessHeap(), 
			HEAP_ZERO_MEMORY, 
			m_SocketSession.PerIoRecv.szDataBuffer, 
			m_nRecvBufferSize);
	}
	else
	{
		m_SocketSession.PerIoRecv.szDataBuffer = (char*)HeapAlloc(
			GetProcessHeap(), 
			HEAP_ZERO_MEMORY, 
			m_nRecvBufferSize);
	}

	//关联Socket到完成端口，句柄数据在此时被绑定到完成端口
	m_SocketSession.PerHandle.hSocket = m_hSocket;
	m_pSocketRunner->AssociateSocket(m_hSocket, &(m_SocketSession.PerHandle));

	//由上层初始化Socket(一般为设置Socket附加信息的指针)
	m_pSocketEvent->OnInitSocket(&(m_SocketSession.PerHandle));

	//重置IO操作数据
	DWORD nFlag = 0;
	DWORD nRecvByte = 0;
	ZeroMemory(&(m_SocketSession.PerIoRecv.Overlapped),sizeof(OVERLAPPED));
	ZeroMemory(m_SocketSession.PerIoRecv.szDataBuffer, m_nRecvBufferSize);
	m_SocketSession.PerIoRecv.SocketBuf.buf = m_SocketSession.PerIoRecv.szDataBuffer;
	m_SocketSession.PerIoRecv.SocketBuf.len = m_nRecvBufferSize;
	m_SocketSession.PerIoRecv.OperType = RECV_POSTED;
	//提交PostRecv请求
	nRet = WSARecv(m_hSocket,&(m_SocketSession.PerIoRecv.SocketBuf),1,
		&nRecvByte,&nFlag,&(m_SocketSession.PerIoRecv.Overlapped),NULL);

	m_bIsConnected	= true;
	DoOpen();

#ifdef SHOW_PRINTFINFO
	printf("TcpClient::Open success socket:%ld, servername=%s, serverport:%d, localport=%d!\r\n",
		m_hSocket, szServerName, nServerPort, nLocalPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
	char szLog[MAX_LOGBUFFER_LENGTH] = {0};
	sprintf(szLog,"TcpClient::Open success socket:%ld, servername=%s, serverport:%d, localport=%d!",
		m_hSocket, szServerName, nServerPort, nLocalPort);
	m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	return true;
}

inline bool CTcpClient::Close()
{	
	SYNC::CGuard<SYNC::CThreadMutex> guard(m_lock);
	if (m_hSocket != INVALID_SOCKET)
	{
		m_bIsConnected = false;
		shutdown(m_hSocket,SD_BOTH);
		closesocket(m_hSocket);
		m_pSocketEvent->OnDisconnect(this, m_hSocket, m_SocketSession.PerHandle.pAttachInfo);
		m_SocketSession.PerHandle.pAttachInfo = NULL;
		m_hSocket = INVALID_SOCKET;

#ifdef SHOW_PRINTFINFO
		printf("TcpClient::Close serverip=%s, serverport:%d, localport=%d!\r\n",
			m_strServerName.c_str(), m_nServerPort, m_nLocalPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
		char szLog[MAX_LOGBUFFER_LENGTH]={0};
		sprintf(szLog,"TcpClient::Close serverip=%s, serverport:%d, localport=%d!",
			m_strServerName.c_str(), m_nServerPort, m_nLocalPort);
		m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	}
	return true;
}

inline SOCKET CTcpClient::GetSocketHandle() const
{
	return m_hSocket;
}

inline bool CTcpClient::DoOpen()
{
	return true;
}

}
