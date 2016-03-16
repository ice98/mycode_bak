//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#include ".\tcpserver.h"
#include "SocketEvent.h"
#include "SocketAPI.h"
#include <assert.h>
namespace SL
{

CTcpServer::CTcpServer(void)
	: m_bIsStart(false)
	, m_nMaxSocketNum(MAX_SOCKET_CONNECTNUM)
	, m_nMaxFreePoolNum(MAX_SOCKET_FREENUM)
	, m_nDefaultFreePoolNum(10)
	, m_hListenSocket(NULL)
{
	InitializeCriticalSection(&m_lock);
}

CTcpServer::~CTcpServer(void)
{
	Close();
	DeleteCriticalSection(&m_lock);
}

inline bool CTcpServer::PostRecv(SOCKET hClientSocket, void *p)
{
	//if ( (hClientSocket == INVALID_SOCKET) || (p == NULL) || (!m_bIsStart) )
	//{
	//	return false;
	//}
	PPER_IO_OPERATION_DATA pPerIoData = (PPER_IO_OPERATION_DATA)p;
	//EnterCriticalSection(&(pPerIoData->IoLock));

	//重置IO操作数据
	DWORD nFlag = 0;
	DWORD nRecvByte = 0;
	ZeroMemory(&(pPerIoData->Overlapped),sizeof(OVERLAPPED));
	//ZeroMemory(pPerIoData->szDataBuffer, m_nRecvBufferSize);
	pPerIoData->SocketBuf.buf = pPerIoData->szDataBuffer;
	pPerIoData->SocketBuf.len = m_nRecvBufferSize;
	pPerIoData->OperType = RECV_POSTED;

	//提交PostRecv请求
	int nRet = WSARecv(hClientSocket, &(pPerIoData->SocketBuf), 1,
		&nRecvByte, &nFlag, &(pPerIoData->Overlapped), NULL);

	//LeaveCriticalSection(&(pPerIoData->IoLock));
	if ( nRet==SOCKET_ERROR ) 
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{//接收失败,最有可能是Socket已断开,所以按断开处理
			Disconnect(hClientSocket);
			return false;
		}
	}
	return true;
}

void CTcpServer::SetConfig(int nMaxSocketNum, int nMaxFreePoolNum, int nDefaultFreePoolNum)
{
	m_nMaxSocketNum = nMaxSocketNum;
	m_nMaxFreePoolNum = nMaxFreePoolNum;
	m_nDefaultFreePoolNum = nDefaultFreePoolNum;
	return;
}

void CTcpServer::SetInterface(CAppLog *pAppLog, CSocketEvent *pSocketEvent, CSocketRunner *pSocketRunner)
{
	assert(pAppLog != NULL);
	assert(pSocketEvent != NULL);
	assert(pSocketRunner != NULL);
	m_pAppLog = pAppLog;
	m_pSocketEvent = pSocketEvent;
	m_pSocketRunner = pSocketRunner;
	return;
}

inline void CTcpServer::SetBufferSize(int nRecvBufferSize, int nSendBufferSize)
{
	if (nRecvBufferSize > 0)
		m_nRecvBufferSize = nRecvBufferSize;
	return;
}

unsigned int WINAPI CTcpServer::ListenProc(LPVOID lParam)
{
	CTcpServer *pSvr = (CTcpServer*)lParam;
	SOCKET      hClientSocket;
	SOCKADDR_IN siClientSocketAddr;
	int         nClientSocketLen;

	char        szIPAddr[MAX_IPADDR_LENGTH];
	USHORT      nPort = 0;
#ifdef SOCKETLIB_WRITE_LOG
	char        szLog[MAX_LOGBUFFER_LENGTH];
#endif

	nClientSocketLen   = sizeof(siClientSocketAddr);
	DWORD dwStartTime  = 0;
	DWORD dwAcceptTime = 0;
	for (;;)
	{
		if (pSvr->m_threadListen.IsStop())
		{//退出线程
#ifdef SHOW_PRINTFINFO
			printf("ListenProc thread exit!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
			pSvr->m_pAppLog->WriteLog("ListenProc thread exit!",__FILE__,__LINE__,CAppLog::LOG_LEVEL_7);
#endif
#endif
			break;
		}

		//置0
		memset(szIPAddr, 0, sizeof(szIPAddr));
#ifdef SOCKETLIB_WRITE_LOG
		memset(szLog, 0, sizeof(szLog));
#endif

		//接收客户的请求
		hClientSocket = WSAAccept(pSvr->m_hListenSocket,(SOCKADDR*)&siClientSocketAddr,&nClientSocketLen,NULL,0);
		if (INVALID_SOCKET == hClientSocket)
		{
#ifdef SHOW_PRINTFINFO
			printf("WSAAccept invalid socket!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
			pSvr->m_pAppLog->WriteLog("WSAAccept invalid socket!",__FILE__,__LINE__,CAppLog::LOG_LEVEL_7);
#endif
#endif
			continue;
		}

		//打印显示信息
		CSocketAPI::GetSocketInfo(hClientSocket, szIPAddr, &nPort);
		if ( pSvr->m_mapSocketSession.size() >= pSvr->m_nMaxSocketNum )
		{
			closesocket(hClientSocket);
#ifdef SHOW_PRINTFINFO
			printf("Socket: %d, clientip:%s, port: %d, refuse; current online total:%ld \r\n", 
				hClientSocket, szIPAddr, nPort, pSvr->m_mapSocketSession.size());
#else
#ifdef SOCKETLIB_WRITE_LOG
			sprintf(szLog,"ListenSocket:%ld, clientip:%s, port: %d, refuse; current online total:%ld",
				pSvr->m_hListenSocket, szIPAddr, nPort, pSvr->m_mapSocketSession.size());
			pSvr->m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
			continue;
		}

		//设置新socket的选项(如接收/发送缓冲区的大小或关闭缓冲区等)
		//int nBufferLen = RECV_BUFFER_SIZE;
		//setsockopt( hSocket, SOL_SOCKET, SO_SNDBUF, (char *) &nBufferLen, sizeof(int) );
		//setsockopt( hSocket, SOL_SOCKET, SO_RCVBUF, (char *) &nBufferLen, sizeof(int) );

		if ( !pSvr->m_pSocketEvent->OnAccept(pSvr, hClientSocket) )
		{
#ifdef SHOW_PRINTFINFO
			printf("ListenSocket:%ld, clientip:%s, port:%d, acceptconnect error!\r\n", 
				pSvr->m_hListenSocket, szIPAddr, nPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
			sprintf(szLog,"ListenSocket:%ld, clientip:%s, port:%d, acceptconnect error!", 
				pSvr->m_hListenSocket, szIPAddr, nPort);
			pSvr->m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif
			closesocket(hClientSocket);
			continue;
		}

		PSOCKET_SESSION pSocketSession = NULL;
		//取得空闲Socket的内存
		EnterCriticalSection(&(pSvr->m_lock));
		if ( !pSvr->m_lstFreePool.empty() )
		{
			pSocketSession = pSvr->m_lstFreePool.front();
			pSocketSession->PerHandle.hSocket		= hClientSocket;
			pSocketSession->PerHandle.pSocketSource = pSvr;
			ZeroMemory(&(pSocketSession->PerIoRecv.Overlapped), sizeof(OVERLAPPED));
			ZeroMemory(&(pSocketSession->PerIoSend.Overlapped), sizeof(OVERLAPPED));
			//ZeroMemory(pSocketSession->PerIoRecv.szDataBuffer, pSvr->m_nRecvBufferSize);
			pSvr->m_lstFreePool.pop_front();
		}
		LeaveCriticalSection(&(pSvr->m_lock));
		if (NULL == pSocketSession)
		{//分配Socket I/O操作数据的内存
			char *p = (char*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, (sizeof(SOCKET_SESSION)+pSvr->m_nRecvBufferSize) );
			if (NULL == p)
			{
#ifdef SHOW_PRINTFINFO
				printf("ListenProc:: heapAlloc memory error!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
				pSvr->m_pAppLog->WriteLog("ListenProc:: heapAlloc memory error!",__FILE__,__LINE__,CAppLog::LOG_LEVEL_7);
	#endif
#endif
				closesocket(hClientSocket);
				continue;
			}
			pSocketSession = (PSOCKET_SESSION)p;
			pSocketSession->PerHandle.hSocket		= hClientSocket;
			pSocketSession->PerHandle.pSocketSource = pSvr;
			pSocketSession->PerIoRecv.szDataBuffer  = (p+sizeof(SOCKET_SESSION));
			InitializeCriticalSection(&(pSocketSession->PerIoSend.IoLock));
		}

		//将Socket关联到完成端口，句柄数据在此时被绑定到完成端口
		pSvr->m_pSocketRunner->AssociateSocket(hClientSocket, &(pSocketSession->PerHandle));

		//加入Socket集合
		EnterCriticalSection(&(pSvr->m_lock));
		pSvr->m_mapSocketSession[hClientSocket] = pSocketSession;
		LeaveCriticalSection(&(pSvr->m_lock));

		//由上层初始化Socket(一般为设置Socket附加信息的指针)
		pSvr->m_pSocketEvent->OnInitSocket(&(pSocketSession->PerHandle));

		//提交首个接收数据请求
		//这时如果客户端断开连接,则也可以接收数据时得到通知
		DWORD nFlag = 0;
		DWORD nRecvByte = 0 ;
		pSocketSession->PerIoRecv.SocketBuf.buf = pSocketSession->PerIoRecv.szDataBuffer;
		pSocketSession->PerIoRecv.SocketBuf.len = pSvr->m_nRecvBufferSize;
		pSocketSession->PerIoRecv.OperType = RECV_POSTED;
		WSARecv(hClientSocket,&(pSocketSession->PerIoRecv.SocketBuf),1,
			&nRecvByte,&nFlag,&(pSocketSession->PerIoRecv.Overlapped),NULL);

#ifdef SHOW_PRINTFINFO
		printf("ListenSocket:%ld, clientip:%s, port:%d, connected; total:%ld\r\n",
			pSvr->m_hListenSocket, szIPAddr, nPort, pSvr->m_mapSocketSession.size());
#else
	#ifdef SOCKETLIB_WRITE_LOG
		sprintf(szLog,"ListenSocket:%ld, clientip:%s, port:%d, connected; total:%ld",
			pSvr->m_hListenSocket, szIPAddr, nPort, pSvr->m_mapSocketSession.size());
		pSvr->m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_5);
	#endif
#endif

	}

	return 0;
}

bool CTcpServer::Open(const char *szServerName, unsigned short nPort, int nBacklog)
{
	//创建侦听Socket
	m_hListenSocket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET==m_hListenSocket)
	{
#ifdef SHOW_PRINTFINFO
		printf("Create listen socket failure!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("Create listen socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
	#endif
#endif
		return false;
	}

	SOCKADDR_IN InternetAddr;
	memset(&InternetAddr,0,sizeof(InternetAddr));
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(nPort);
	if (bind(m_hListenSocket,(SOCKADDR*)&InternetAddr,sizeof(InternetAddr)) == SOCKET_ERROR)
	{
#ifdef SHOW_PRINTFINFO
		printf("TcpServer bind socket failure!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("TcpServer bind socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
	#endif
#endif
		return false;
	}

	//侦听端口上的连接请求
	if ( listen(m_hListenSocket,nBacklog) == SOCKET_ERROR )
	{
#ifdef SHOW_PRINTFINFO
		printf("Listening socket failure!\r\n");
#else
	#ifdef SOCKETLIB_WRITE_LOG
		m_pAppLog->WriteLog("Listening socket failure!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
	#endif
#endif
		return false;
	}

	//创建空闲Soket的内存池
	for (int i=0; i<m_nDefaultFreePoolNum; i++)
	{
		//分配IO操作数据的内存
		char *p = (char*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 
			(sizeof(SOCKET_SESSION)+m_nRecvBufferSize) );
		if (NULL == p)
		{
			return false;
		}
		PSOCKET_SESSION pSocketSession = (PSOCKET_SESSION)p;
		pSocketSession->PerHandle.hSocket		= 0;
		pSocketSession->PerHandle.pSocketSource = this;
		pSocketSession->PerIoRecv.szDataBuffer  = (p+sizeof(SOCKET_SESSION));
		InitializeCriticalSection(&(pSocketSession->PerIoSend.IoLock));
		m_lstFreePool.push_back(pSocketSession);
	}
	m_bIsStart = true;

	//启动侦听线程
	m_threadListen.Start(CTcpServer::ListenProc,this,false);

#ifdef SHOW_PRINTFINFO
	printf("TcpServer::Open success, socket:%ld port:%d!\r\n", m_hListenSocket, nPort);
#else
#ifdef SOCKETLIB_WRITE_LOG
	SL_TCHAR szLog[MAX_LOGBUFFER_LENGTH]={0};
	_stprintf(szLog, TEXT("TcpServer::Open success, socket:%ld port:%d!"), m_hListenSocket, nPort);
	m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	return true;
}

bool CTcpServer::Close()
{
	m_bIsStart = false;
	if (m_hListenSocket == NULL)
		return false;

	//关闭网络的侦听
	shutdown(m_hListenSocket,0);
	closesocket(m_hListenSocket);
	m_hListenSocket = NULL;

	m_threadListen.Stop(100);
	m_threadListen.Kill();

	//清除在线Socket
	std::map<SOCKET,PSOCKET_SESSION>::iterator itClientSocket;
	for (itClientSocket = m_mapSocketSession.begin(); itClientSocket != m_mapSocketSession.end(); itClientSocket++)
	{
		shutdown(itClientSocket->first,SD_BOTH);
		closesocket(itClientSocket->first);
		DeleteCriticalSection(&(itClientSocket->second->PerIoSend.IoLock));
		HeapFree(GetProcessHeap(),0,(void*)itClientSocket->second);
	}
	m_mapSocketSession.clear();

	//清除空闲池
	std::list<PSOCKET_SESSION>::iterator itItem;
	for (itItem = m_lstFreePool.begin(); itItem != m_lstFreePool.end(); itItem++)
	{
		DeleteCriticalSection(&((*itItem)->PerIoSend.IoLock));
		HeapFree(GetProcessHeap(),0,*itItem);
	}
	m_lstFreePool.clear();

#ifdef SHOW_PRINTFINFO
	printf("TcpServer::Close()!\r\n");
#else
#ifdef SOCKETLIB_WRITE_LOG
	m_pAppLog->WriteLog("TcpServer::Close()!", __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
#endif
#endif

	return true;
}

inline bool CTcpServer::Disconnect(SOCKET hClientSocket, bool bForce)
{
	if ( (hClientSocket==INVALID_SOCKET) || (hClientSocket==0) )
	{
		return false;
	}
	void *pAttachInfo = NULL;

	//取得Socket操作数据
	EnterCriticalSection(&m_lock);
	std::map<SOCKET,PSOCKET_SESSION>::iterator itClientSocket;
	itClientSocket = m_mapSocketSession.find(hClientSocket);
	if ( itClientSocket == m_mapSocketSession.end() )
	{
		LeaveCriticalSection(&m_lock);
		return false;
	}
	PSOCKET_SESSION pSocketSession = itClientSocket->second;
	pAttachInfo = pSocketSession->PerHandle.pAttachInfo;

	char   szIPAddr[MAX_IPADDR_LENGTH]={0};
	USHORT nPort = 0;
	CSocketAPI::GetSocketInfo(hClientSocket, szIPAddr, &nPort);
#ifdef SHOW_PRINTFINFO
	printf("ListenSocket:%ld, socket:%ld, clientip:%s, client close; tootal:%ld\r\n", 
		m_hListenSocket, hClientSocket, szIPAddr, m_mapSocketSession.size()-1 );
#else
#ifdef SOCKETLIB_WRITE_LOG
	char szLog[MAX_LOGBUFFER_LENGTH];
	sprintf(szLog, "ListenSocket:%ld, socket:%ld, clientip:%s, client close; tootal:%ld", 
		m_hListenSocket, hClientSocket, szIPAddr, m_mapSocketSession.size()-1 );
	m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_5);
#endif
#endif

	//关闭套接字
	shutdown(hClientSocket,SD_BOTH);
	closesocket(hClientSocket);

	if (m_lstFreePool.size() >= m_nMaxFreePoolNum)
	{
		DeleteCriticalSection(&(pSocketSession->PerIoSend.IoLock));
		HeapFree(GetProcessHeap(),0,(void*)pSocketSession);
	}
	else
	{
		m_lstFreePool.push_back(pSocketSession);
	}
	m_mapSocketSession.erase(hClientSocket);
	LeaveCriticalSection(&m_lock);

	if (!bForce)
		m_pSocketEvent->OnDisconnect(this, hClientSocket, pAttachInfo);

	return true;
}

inline int CTcpServer::Recv(SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen)
{
	return m_pSocketEvent->OnRecv(this, hClientSocket, pAttachInfo, szData, nLen);
}

int CTcpServer::Send(SOCKET hClientSocket, const char *szData, int nLen)
{
	//if ( (hClientSocket==INVALID_SOCKET) || (szData==NULL) || (nLen<=0) || (!m_bIsStart) )
	//{
	//	return -1;
	//}

	//取得Socket操作数据
	EnterCriticalSection(&m_lock);
	std::map<SOCKET,PSOCKET_SESSION>::iterator itClientSocket;
	itClientSocket = m_mapSocketSession.find(hClientSocket);
	if ( itClientSocket == m_mapSocketSession.end() )
	{
		LeaveCriticalSection(&m_lock);
		return -1;
	}
	LeaveCriticalSection(&m_lock);
	PSOCKET_SESSION pSocketSession = itClientSocket->second;
	EnterCriticalSection(&(pSocketSession->PerIoSend.IoLock));

	//重置缓冲区
	DWORD nFlag = 0;
	DWORD nSendByte = 0;
	ZeroMemory(&(pSocketSession->PerIoSend.Overlapped),sizeof(OVERLAPPED));
	pSocketSession->PerIoSend.SocketBuf.buf = (char*)szData;
	pSocketSession->PerIoSend.SocketBuf.len = nLen;
	pSocketSession->PerIoSend.OperType = SEND_POSTED;
	int nRet = WSASend(hClientSocket,&(pSocketSession->PerIoSend.SocketBuf),1,
		&nSendByte,nFlag,&(pSocketSession->PerIoSend.Overlapped),NULL);

	LeaveCriticalSection(&(pSocketSession->PerIoSend.IoLock));
	if ( nRet==SOCKET_ERROR )
	{
		int nErrorID = WSAGetLastError();
		if (nErrorID != WSA_IO_PENDING)
		{//发送失败,最有可能是Socket已断开,所以按断开处理
			Disconnect(pSocketSession->PerHandle.hSocket);
#ifdef SHOW_PRINTFINFO
			printf("TcpServer::Send error:%ld, ListenSocket:%ld! \r\n", nErrorID, m_hListenSocket);
#else
	#ifdef SOCKETLIB_WRITE_LOG 
			char   szLog[MAX_LOGBUFFER_LENGTH];
			sprintf(szLog,"TcpServer::Send error:%ld, ListenSocket:%ld!",nErrorID, m_hListenSocket);
			m_pAppLog->WriteLog(szLog, __FILE__, __LINE__, CAppLog::LOG_LEVEL_7);
	#endif
#endif
			return -1;
		}
	}
	return nSendByte;
}

inline bool CTcpServer::IsConnected(SOCKET hClientSocket)
{
	EnterCriticalSection(&m_lock);
	std::map<SOCKET,PSOCKET_SESSION>::iterator itClientSocket;
	itClientSocket = m_mapSocketSession.find(hClientSocket);
	if ( itClientSocket == m_mapSocketSession.end() )
	{
		LeaveCriticalSection(&m_lock);
		return false;
	}
	LeaveCriticalSection(&m_lock);
	return true;
}

inline bool CTcpServer::SetAttachInfo(SOCKET hClientSocket, void *pAttachInfo)
{
	EnterCriticalSection(&m_lock);
	std::map<SOCKET,PSOCKET_SESSION>::const_iterator itClientSocket;
	itClientSocket = m_mapSocketSession.find(hClientSocket);
	if ( itClientSocket == m_mapSocketSession.end() )
	{
		LeaveCriticalSection(&m_lock);
		return false;
	}
	PSOCKET_SESSION pSocketSession = (*itClientSocket).second;
	pSocketSession->PerHandle.pAttachInfo = pAttachInfo;
	LeaveCriticalSection(&m_lock);
	return false;
}

void* CTcpServer::GetAttachInfo(SOCKET hClientSocket)
{
	EnterCriticalSection(&m_lock);
	std::map<SOCKET,PSOCKET_SESSION>::iterator itClientSocket;
	itClientSocket = m_mapSocketSession.find(hClientSocket);
	if ( itClientSocket == m_mapSocketSession.end() )
	{
		LeaveCriticalSection(&m_lock);
		return NULL;
	}
	PSOCKET_SESSION pSocketSession = (*itClientSocket).second;
	LeaveCriticalSection(&m_lock);
	return pSocketSession->PerHandle.pAttachInfo;
}

}