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
#include "../config.h"
#include "guard.h"
namespace SYNC
{
class SOCKETLIB_API CThreadManager
{
public:
	CThreadManager();
	~CThreadManager();

protected:
	CThreadMutex		m_lock;

};

};