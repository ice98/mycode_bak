//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

// -------------------------------------------------------------------------
// class CRefcount - 引用计数

#ifndef REF_COUNT_H
#define REF_COUNT_H

#pragma once
#include "../config.h"

namespace SLEXT
{

class SOCKETLIB_API CRefcount
{
public:
	// Destructor.
	virtual ~CRefcount (void);

	// = Increment/Decrement refcount
	int increment (void);
	int decrement (void);

	// Returns the current refcount.
	int refcount (void) const;

protected:
	// Protected constructor.
	CRefcount (int refcount);

	// Current refcount.
	int m_nRefcount;
};

};

#endif