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
#include <malloc.h>
#include <memory.h>

namespace SLEXT
{

//the owner option is used when the CByteBuffer object can't 
//keep a copy of the data available
class SOCKETLIB_API CByteBuffer
{
public:
	typedef char value_type;
	typedef char* pointer;
	typedef const char* const_pointer;
	typedef char* iterator;
	typedef const char* const_iterator;
	typedef std::size_t size_type;

private:
	CByteBuffer(const CByteBuffer& oByteBuffer);
	CByteBuffer &operator= (const CByteBuffer &);

public:
	CByteBuffer()
		: m_szData(0)
		, m_nSize(0)
		, m_bOwner(false)
		, m_nCapacity(0)
	{
	};
	CByteBuffer(size_t nCapacity)
		: m_szData(0)
		, m_nSize(0)
		, m_bOwner(true)
		, m_nCapacity(nCapacity)
	{
		m_szData = (char*)malloc(nCapacity);
	};

	CByteBuffer(const char *szData, size_t nSize, bool bOwner = false)
		: m_nCapacity(0)
	{
		m_nSize	 = nSize;
		m_bOwner = bOwner;
		if (bOwner)
		{
			m_szData = (char*)malloc(nSize);
			memcpy(m_szData, szData, nSize);
		}
		else
			m_szData = (char*)szData;
	};

	virtual ~CByteBuffer()
	{
		clear();
	};

	bool owner() const
	{
		return m_bOwner;
	}
	void owner(bool bOwner)
	{
		m_bOwner = bOwner;
	}

	iterator data()
	{
		return m_szData;
	};
	const_iterator data() const
	{
		return m_szData;
	};
	void data(const char *szData, size_t nSize, bool bOwner = false)
	{
		if (szData == 0)
			return;
		if (szData == m_szData)
			return;

		clear();
		m_nSize	 = nSize;
		m_bOwner = bOwner;
		if (bOwner)
		{
			m_szData = (char*)malloc(nSize);
			memcpy(m_szData,szData,nSize);
		}
		else
			m_szData = (char*)szData;
	};
	void data(CByteBuffer& oBuffer, bool bMoveOwner = true)
	{
		clear();
		m_nSize	= oBuffer.size();
		if ( (bMoveOwner) && oBuffer.owner() )
		{
			oBuffer.owner(false);
			m_bOwner = true;
		}
		else
		{
			m_bOwner = false;
		}
		m_szData = oBuffer.data();
	};

	size_type capacity() const
	{
		return m_nCapacity;
	};
	size_type size() const
	{
		return m_nSize;
	};

	void clear()
	{
		if ( (m_bOwner) && (m_szData != 0) )
		{
			::free(m_szData);
		}
		m_szData = 0;
		m_nSize  = 0;
		m_bOwner = false;
		m_nCapacity = 0;
	};
	bool empty() const
	{
		return m_nSize == 0;
	};
	iterator begin()
	{
	    return m_szData;
	};
	const_iterator begin() const
	{
	    return m_szData;
	};
	iterator end()
	{
	    return m_szData + m_nSize;
	};
	const_iterator end() const
	{
	    return m_szData + m_nSize;
	};

	bool append(const char *szData, size_t nSize)
	{
		if (!m_bOwner)
			return false;

		if (m_nCapacity >= nSize)
		{
			memcpy(m_szData+m_nSize, szData, nSize);
			m_nCapacity -= nSize;
		}
		else
		{
			char *pNew =(char*)malloc(m_nSize+nSize);
			memcpy(pNew, m_szData, m_nSize);
			memcpy(pNew+m_nSize, szData, nSize);
			::free(m_szData);
			m_szData	= pNew;
			m_nCapacity = 0;
		}
		m_nSize += nSize;
		return true;
	};

private:
	bool		 m_bOwner;		//true:表示Buffer的所有权为owner
	std::size_t  m_nSize;       //当前占用Buffer大小
	std::size_t  m_nCapacity;   //Buffer的容量
	char		*m_szData;      //缓冲区
};

};