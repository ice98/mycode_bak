#pragma once
#include "../../socketlib/socketevent.h"
#include "../../socketlib/socketsource.h"

using namespace SL;
class CTestSocketEvent : public CSocketEvent
{
public:
	CTestSocketEvent(void);
	virtual ~CTestSocketEvent(void);

	bool OnAccept(CSocketSource *pSource, SOCKET hClientSocket);
	bool OnDisconnect(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo);
	int  OnRecv(CSocketSource *pSource, SOCKET hClientSocket, void *pAttachInfo, const char *szData, int nLen);

	int  OnRecv(CSocketSource *pSource, const char *szData, int nLen, sockaddr *pFrom, int nFromLen);

};
