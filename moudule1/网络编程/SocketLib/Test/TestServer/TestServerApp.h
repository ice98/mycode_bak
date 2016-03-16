#pragma once
#include "../../socketlib/appbase.h"
#include "../../socketlib/applog.h"
#include "../../socketlib/tcprunner.h"
#include "../../socketlib/tcpserver.h"
#include "../../socketlib/tcpclient.h"
#include "TestSocketEvent.h"

using namespace SL;
class CTestServerApp : public CAppBase
{
public:
	CTestServerApp(void);
	~CTestServerApp(void);

	virtual void DoReadyWork(); //����ǰ��׼������(������NT��������)
	virtual bool DoRun();
	virtual bool DoStop();

private:
	CAppLog				m_appLog;
	CTcpRunner			m_tcpRunner;
	CTcpServer			m_tcpServer;
	CTestSocketEvent	m_testSocketEvent;
};
