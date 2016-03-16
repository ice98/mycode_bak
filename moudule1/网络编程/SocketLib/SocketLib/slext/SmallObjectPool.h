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

#include <list>
#include "../sync/guard.h"

namespace SLEXT
{

template <typename T, typename Mutex>
class CSmallObjectPool
{
private:
    CSmallObjectPool(const CSmallObjectPool &);
    void operator= (const CSmallObjectPool &);

public:
	CSmallObjectPool()
		: m_nMaxCount(0),m_nInitCount(0),m_nGrowCount(1)
	{
	};
	virtual ~CSmallObjectPool()
	{
		clear();
	};

	void init(unsigned int nMaxCount,unsigned int nInitCount, unsigned int nGrowCount)
	{
		m_nMaxCount  = nMaxCount;
		m_nInitCount = nInitCount;
		m_nGrowCount = nGrowCount;
		_AllocT(nInitCount);
	};
	
	T* pop()
	{
		SYNC::CGuard<Mutex> guard(m_mtx);
        T* pObj = NULL;
		if (!m_objlist.empty())
        {
            pObj = m_objlist.front();
            m_objlist.pop_front();
        }
		else
		{
			pObj = new T();
			_AllocT(m_nGrowCount);
		};
		return pObj;
	};

    void push(T* pObj)
    {
		SYNC::CGuard<Mutex> guard(m_mtx);
		if (m_objlist.size() < m_nMaxCount)
			m_objlist.push_back(pObj);
		else
			delete pObj;
    };

	void clear()
	{
		SYNC::CGuard<Mutex> guard(m_mtx);
		std::list<T*>::iterator itr = m_objlist.begin();
        for (; itr != m_objlist.end(); itr++)
        {
            delete *itr;
            *itr = NULL;
        }
		m_objlist.clear();
	};

private:
	bool _AllocT(unsigned int nCount)
	{
		T *pObj = NULL;
		for (unsigned i = 0; i < nCount; i++)
		{			
			pObj = new T();
			m_objlist.push_back(pObj);
		}
		return true;
	};

private:
	Mutex		  m_mtx;
	std::list<T*> m_objlist;

	unsigned int  m_nMaxCount;
    unsigned int  m_nInitCount;
    unsigned int  m_nGrowCount;
};

};