#include "StdAfx.h"
#include ".\testsocketevent.h"
#include "..\..\socketlib\slcommondef.h"
#include "..\..\socketlib\socketapi.h"

CTestSocketEvent::CTestSocketEvent(void)
{
}

CTestSocketEvent::~CTestSocketEvent(void)
{
}

bool CTestSocketEvent::OnAccept(CSocketSource *pSource, SOCKET hClientSocket)
{
	SL::CSocketAPI::SetSocketKeepAlive(hClientSocket,1);
	SL::CSocketAPI::SetSocketKeepAliveValue(hClientSocket,900000,10000);
	return true;
}

bool CTestSocketEvent::OnDisconnect(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo)
{
	return true;
}

int CTestSocketEvent::OnRecv(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen)
{
	printf("Socket:%ld msg:(%s) len:%d\r\n", hClientSocket, szData, nLen);

	//将收到的数据直接发回客户
	int nRet = pSource->Send(hClientSocket, szData, nLen);
	printf("Socket:%ld sendmsg:(%s) len:%d\r\n", hClientSocket, szData, nRet);
	return 1;
}

int CTestSocketEvent::OnRecv(CSocketSource *pSource, const char *szData, int nLen, 
							 sockaddr *pFrom, int nFromLen)
{
	return 1;
}
