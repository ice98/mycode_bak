//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef APPBASE_H
#define APPBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "config.h"
#include "NTService.h"
namespace SL
{

class SOCKETLIB_API CAppBase : public CNTService
{
public:
	CAppBase(LPCTSTR pszServiceName, LPCTSTR pszDisplayName);
	virtual ~CAppBase();

	virtual void DoReadyWork(); //运行前的准备工作(如设置NT服务名等)
	virtual bool DoRun();
	virtual bool DoStop();

protected:
	virtual void Run(DWORD dwArgc, LPTSTR *ppszArgv);//Service Mainloop
	virtual void Stop();//Stop service

protected:
	HANDLE			m_hStop;//NTService used

};

};

#endif