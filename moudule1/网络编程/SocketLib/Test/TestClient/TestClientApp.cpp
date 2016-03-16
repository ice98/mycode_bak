#include "StdAfx.h"
#include ".\testclientapp.h"
#include <string>

// CAccountSrvApp ����
#ifdef _DEBUG
CTestClientApp::CTestClientApp()
	: CAppBase(TEXT("TestClient"), TEXT("Socketlib test client"))
#else
CTestClientApp::CTestClientApp()
	: CAppBase(TEXT("TestClient"), TEXT("Socketlib test client"))
#endif
{
#ifdef _DEBUG
	SetupConsole();
#endif
	DoReadyWork();
};

CTestClientApp::~CTestClientApp()
{
	//DoStop();
}

//����ǰ��׼������(������NT��������)
void CTestClientApp::DoReadyWork()
{
	return;
}

bool CTestClientApp::DoRun()
{
	std::string str;

	m_tcpRunner.Open(0,4);
	m_appLog.OpenLog("C:\\log","cli",CAppLog::LOG_LEVEL_5,3);

	m_tcpTest.SetInterface(&m_appLog,&m_testSocketEvent,&m_tcpRunner);
	m_tcpTest.SetBufferSize(1024,1024);
	m_tcpTest.Open("127.0.0.1", 3050, 0);

	m_udpRunner.Open(0,2);
	m_udpClient.SetInterface(&m_appLog,&m_testSocketEvent,&m_udpRunner);
	m_udpClient.SetBufferSize(1024,1024);
	m_udpClient.Open("127.0.0.1", 3050, 0);

	//����Ƶ��������
	str = "<msg>hello,channelserver</msg>";
	SL_MSGHEAD_EX msgHead;
	msgHead.nSize		= sizeof(SL_MSGHEAD_EX) + str.size();
	msgHead.nCommand	= 60001;
	msgHead.nParams[0]  = 60001;

	m_tcpTest.Send((char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	m_tcpTest.Send(str.c_str(),str.size());
	m_tcpTest.Send((char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	m_tcpTest.Send(str.c_str(),str.size());
	m_tcpTest.Send((char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	m_tcpTest.Send(str.c_str(),str.size());
	m_tcpTest.Send((char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	m_tcpTest.Send(str.c_str(),str.size());
	m_tcpTest.Send((char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	m_tcpTest.Send(str.c_str(),str.size());

	//
	char szSend[260];
	memcpy(szSend,(char*)&msgHead,sizeof(SL_MSGHEAD_EX));
	memcpy(szSend+sizeof(SL_MSGHEAD_EX), str.c_str(), str.size());
	m_udpClient.Send(szSend, msgHead.nSize);
	m_udpClient.Send(szSend, msgHead.nSize);
	m_udpClient.Send(szSend, msgHead.nSize);
	m_udpClient.Send(szSend, msgHead.nSize);
	m_udpClient.Send(szSend, msgHead.nSize);

	//str = "Welcome you use socketlib";
	//m_tcpTest.Send(str.c_str(), str.size(), false);
	//Sleep(10);
	//m_tcpTest.Send(str.c_str(), str.size(), false);
	//Sleep(10);
	//m_tcpTest.Send(str.c_str(), str.size(), false);
	//m_tcpTest.Send(str.c_str(), str.size(), false);
	//Sleep(10);

	////���´���������ӶϿ����Զ�����������
	//m_tcpTest.Close();
	//m_tcpTest.Send(str.c_str(), str.size(), true);
	////Sleep(10);
	//m_tcpTest.Send(str.c_str(), str.size(), true);
	////Sleep(10);
	//m_tcpTest.Send(str.c_str(), str.size(), true);
	////Sleep(10);
	//m_tcpTest.Send(str.c_str(), str.size(), true);

	////����httpЭ��
	//m_tcpHttp.SetInterface(&m_appLog,&m_testSocketEvent,&m_tcpRunner);
	//m_tcpHttp.SetBufferSize(1024,1024);
	//m_tcpHttp.Open("www.google.com",80,0);
	//str = "GET / HTTP/1.1\r\nHost:www.google.com\r\nAccept:*/*\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\nConnection:Keep-Alive\r\n\r\n";
	//m_tcpHttp.Send(str.c_str(), str.size(), false);

	return true;
}

bool CTestClientApp::DoStop()
{
	m_tcpRunner.Close();
	m_tcpHttp.Close();
	m_appLog.WriteLog("app end!",__FILE__,__LINE__,CAppLog::LOG_LEVEL_LAST);
	m_appLog.CloseLog();
	return true;
}

