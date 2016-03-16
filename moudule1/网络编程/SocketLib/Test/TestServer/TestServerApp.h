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

	virtual void DoReadyWork(); //运行前的准备工作(如设置NT服务名等)
	virtual bool DoRun();
	virtual bool DoStop();

private:
	CAppLog				m_appLog;
	CTcpRunner			m_tcpRunner;
	CTcpServer			m_tcpServer;
	CTestSocketEvent	m_testSocketEvent;
};
