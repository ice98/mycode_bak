#ifndef _ICE_BASETYPE_H
#define _ICE_BASETYPE_H
#include <afxwin.h>
// #include <windows.h>
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stack>
#include <iostream>


using namespace std;





#define IN
#define OUT
#define IO

#define SUCCESS	(0)
#define FAILE	(-1)
#define ICY_FILE __FILE__
#define ICY_LINE __LINE__

typedef int HERROR;
typedef vector<HWND> WND_VECTOR;
typedef vector<HWND>::iterator WND_VECTOR_ITERATOR;

template<typename T>
class parent_childv
{
public:
	parent_childv(T value)
	{
		m_hParent = value;
	}
	~parent_childv()
	{
		m_vChild.clear();
	}
	T m_hParent;
	vector<T> m_vChild;
};

typedef parent_childv<HWND> WND_PARENT_CHILDV;

typedef vector<WND_PARENT_CHILDV> V_WND_PARENT_CHILD_V;

typedef vector<HANDLE> V_HANDLE;
#endif