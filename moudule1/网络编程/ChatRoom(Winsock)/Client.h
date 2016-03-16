// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_)
#define AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsock.h"

class CExample1Dlg;

//##ModelId=46F43D0C030D
class CClient  
{
public:
	//##ModelId=46F43D0C031C
	CClient();
	//##ModelId=46F43D0C031D
	virtual ~CClient();
private:
	//##ModelId=46F43D0C032C
	void ClientInit();
public:
	//##ModelId=46F43D0C032D
	BOOL InitAndConnet(HWND hwnd,UINT port,CString m_strServer);
public:
	//##ModelId=46F43D0C033E
	void GetString(CString & str);
	//##ModelId=46F43D0C0340
	void SendString(CString a);
	//##ModelId=46F43D0C034B
	CString m_strServer;
	//##ModelId=46F43D0C035C
	SOCKET m_hSocket;
	//##ModelId=46F43D0C036C
	sockaddr_in m_addr;
	//##ModelId=46F43D0C0370
	UINT m_uPort;
	//##ModelId=46F43D0C0371
    HWND m_hWnd;
};

#endif // !defined(AFX_CLIENT_H__EB7CB9C2_7F60_48B7_BCEB_4884DB3243B9__INCLUDED_)
