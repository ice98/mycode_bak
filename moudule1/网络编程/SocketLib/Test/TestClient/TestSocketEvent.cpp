#include "StdAfx.h"
#include ".\testsocketevent.h"
#include "..\..\socketlib\slcommondef.h"

CTestSocketEvent::CTestSocketEvent(void)
{
}

CTestSocketEvent::~CTestSocketEvent(void)
{
}

bool CTestSocketEvent::OnAccept(CSocketSource *pSource, SOCKET hClientSocket)
{
	return true;
}

bool CTestSocketEvent::OnDisconnect(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo)
{
	return true;
}

int CTestSocketEvent::OnRecv(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen)
{
	//printf("Server return msg:(%s) len:%d\r\n", szData, nLen);
	printf("tcp Srv return:(%s,%ld)\n",szData+40,nLen);
	return 1;
}

int CTestSocketEvent::OnRecv(CSocketSource *pSource, const char *szData, int nLen, 
							 sockaddr *pFrom, int nFromLen)
{
	printf("udp Srv return:(%s,%ld)\n",szData+40,nLen);
	return 1;
}
