//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#pragma once
#include "SocketRunner.h"

namespace SL
{

class SOCKETLIB_API CUdpRunner : public CSocketRunner
{
public:
	CUdpRunner(void);
	virtual ~CUdpRunner(void);

	bool Open(unsigned short nConcurrencyThreadNum = 0, unsigned short nWorkThreadNum = 4);
	bool Close();

private:
	static unsigned int WINAPI Run(LPVOID lParam);

};

};
