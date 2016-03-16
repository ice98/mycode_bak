//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

// AppServer.cpp : Defines the entry point for the DLL application.
//
#include "AppBase.h"
#include "SocketAPI.h"
namespace SL
{

CAppBase::CAppBase(LPCTSTR pszServiceName, LPCTSTR pszDisplayName)
	:CNTService(pszServiceName,pszDisplayName)
{
	DoReadyWork();
}

CAppBase::~CAppBase()
{

}

void CAppBase::Stop()
{
	ReportStatus(SERVICE_STOP_PENDING, 11000);
	if (m_hStop)
		SetEvent(m_hStop);
	return;
};

void CAppBase::Run(DWORD dwArgc, LPTSTR *ppszArgv)
{
	ReportStatus(SERVICE_START_PENDING);
	m_hStop = CreateEvent(0, true, false, 0);
	ReportStatus(SERVICE_RUNNING);
	CSocketAPI::LoadSocketLib(SL_WINSOCK_VERSION);

	//main loop
	if (!DoRun())
		SetEvent(m_hStop);
	
	WaitForSingleObject(m_hStop,INFINITE);

	DoStop();
	CSocketAPI::UnloadSocketLib();
	if (m_hStop)
	{
		CloseHandle(m_hStop);
	}
	return;
}

//运行前的准备工作(如设置NT服务名等)
void CAppBase::DoReadyWork()
{
	return;
}

bool CAppBase::DoRun()
{
	return false;
}

bool CAppBase::DoStop()
{
	return true;
}

}
