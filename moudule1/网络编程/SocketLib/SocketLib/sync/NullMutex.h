//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef NULL_MUTEX_H
#define NULL_MUTEX_H

#pragma once
#include "../config.h"

namespace SYNC
{

class SOCKETLIB_API CNullMutex
{
private:
    CNullMutex(const CNullMutex &);
    void operator= (const CNullMutex &);

public:
	CNullMutex();
	~CNullMutex();
	
	DWORD Wait(DWORD nTimeout);
	bool Lock(DWORD nTimeout);
	bool Lock();
	bool UnLock();
	bool TryLock();
	HANDLE GetHandle() const;

private:
	bool Clear();
};

};

#endif