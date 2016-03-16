#pragma once
#include "../../socketlib/appbase.h"
#include "../../socketlib/applog.h"
#include "../../socketlib/tcprunner.h"
#include "../../socketlib/tcpserver.h"
#include "../../socketlib/tcpclient.h"
#include "../../socketlib/udpsource.h"
#include "../../socketlib/udprunner.h"
#include "../../socketlib/slcommondef.h"
#include "TestSocketEvent.h"

using namespace SL;
class CTestClientApp : public CAppBase
{
public:
	CTestClientApp(void);
	~CTestClientApp(void);

	virtual void DoReadyWork(); //����ǰ��׼������(������NT��������)
	virtual bool DoRun();
	virtual bool DoStop();

private:
	CAppLog				m_appLog;
	CTcpRunner			m_tcpRunner;
	CTcpClient			m_tcpHttp;     //����web��վ
	CTcpClient			m_tcpTest;     //��ͨ����
	CTestSocketEvent	m_testSocketEvent;

	CUdpRunner			m_udpRunner;
	CUdpSource			m_udpClient;

};
