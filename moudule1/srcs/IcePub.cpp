/* !@IcePub.cpp
********************************************************************************
模块名		: 
文件名		: IcePub.cpp
相关文件	: IcePub.h
文件描述	:
作者		: Icy
版本		: 1.0
--------------------------------------------------------------------------------
备注		: 
--------------------------------------------------------------------------------
修改记录 : 
日  期          版本            修改人          修改内容 
2008/03/20     1.0             Icy				创建
********************************************************************************
* 版权所有(c) 2008, 2015, Icy, 保留所有权利
================================================================================
 
*******************************************************************************/

#include "IcePub.h"



/******************************************************************************\
*									引入函数
/******************************************************************************/
extern HERROR IcyEnumWindows(IN WND_VECTOR &wndVctor, IN HWND hwdParent = NULL);

/******************************************************************************\
*									引入变量
/******************************************************************************/

/******************************************************************************\
*									基本函数实现
/******************************************************************************/
BOOL IcyStringMatch(const char* str1, const char* str2)
{
	DWORD ret1 = string(str1).find(str2);
	DWORD ret2 = string(str2).find(str1);
	// string::npos  = -1
	if ((string::npos != ret1) || (string::npos != ret2) )
	{
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
void IcyRefreshWindowHandles()
{

	g_vWnd.clear();
	g_v_wnd_parent_childv.clear();

	WND_VECTOR tmpv;
	IcyEnumWindows(tmpv); //找出顶层；

	DWORD top_size = tmpv.size();
	for (DWORD i = 0; i < top_size; i++)
	{
		g_v_wnd_parent_childv.push_back(tmpv[i]);
		// printf("parent 0x%x\n", tmpv[i]);

		IcyEnumWindows(g_v_wnd_parent_childv[i].m_vChild, tmpv[i]);
	}

	for (i = 0; i < top_size; i++)
	{
		// 顶层压入
		g_vWnd.push_back(g_v_wnd_parent_childv[i].m_hParent);

		// 紧跟其子句柄压入
		for (DWORD j = 0; j < g_v_wnd_parent_childv[i].m_vChild.size(); j++)
		{
			g_vWnd.push_back(g_v_wnd_parent_childv[i].m_vChild[j]);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////

void TestIcyRefreshWindowHandles()
{
	IcyRefreshWindowHandles();
	DWORD top_size = g_v_wnd_parent_childv.size();
	DWORD child_sum1 = 0;
	printf("total %d\n", top_size);
	for (DWORD i = 0; i < top_size; i++)
	{
		DWORD child_size = g_v_wnd_parent_childv[i].m_vChild.size();
		printf("parent:0x%x, count of childs:%d\n", g_v_wnd_parent_childv[i].m_hParent, child_size);

		child_sum1 +=  child_size;
		
		for (DWORD j = 0; j < child_size; j++)
		{
			printf("--0x%x\n",g_v_wnd_parent_childv[i].m_vChild[j]);
		}		
	}

	DWORD total1 = top_size + child_sum1;
	DWORD total2 = g_vWnd.size();

// 	for (i = 0; i < total2; i++)
// 	{
// 		printf("---- 0x%x\n",g_vWnd[i]);
// 	}
	
	printf("top windows: %d, total1 %d, total2 %d\n",top_size, total1, total2);

	
}

///////////////////////////////////////////////////////////////////////////////
