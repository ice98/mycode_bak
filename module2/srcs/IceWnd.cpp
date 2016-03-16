#include "IceWnd.h"

///////////////////////////////////////////////////////////////////////////////////////////////
INT IcyGetWindowText(HWND hWnd, PSTR strBuf, DWORD dwMaxRcv /* = 255 */)
{
	/* 
	If the function succeeds, the return value is the length, in characters, of the copied string, 
	not including the terminating NULL character. If the window has no title bar or text, if the title
	bar is empty, or if the window or control handle is invalid, the return value is zero. To get 
	extended error information, call GetLastError.
	*/

	DWORD dwRet = GetWindowText(hWnd, strBuf, dwMaxRcv);

// 	if (NULL == dwRet)
// 	{
// 		ICY_ASSERT2(FALSE, GetLastError());
// 	}

	return dwRet;

}
///////////////////////////////////////////////////////////////////////////////////////////////

HWND IcyFindWindow(LPCSTR lpWindowName, LPCSTR lpClassName /*= NULL*/)
{
	/* 先用FindWindow()精确查找 */
	HWND hWndRet = FindWindow(lpClassName, lpWindowName);
	if ( NULL != hWndRet) 
	{
		return hWndRet;
	}
	else /* 精确模式找不到，用模糊方式查找 */
	{
		IcyRefreshWindowHandles();
		for (DWORD i = 0; i < g_vWnd.size(); i++)
		{
			CHAR pTextStr[255] = {0};

			INT len = IcyGetWindowText(g_vWnd[i], pTextStr);
	
			if (NULL != lpClassName)
			{
				CHAR pClassName[255] = {0};
				GetClassName(g_vWnd[i], pClassName, 255);
				if ((len > 0) && IcyStringMatch(lpWindowName, pTextStr)
					&& (NULL == strcmp(pClassName, lpClassName)))
				{
					return g_vWnd[i];
				}
			}
			else // 不需要类名匹配
			{
				if ((len > 0) && IcyStringMatch(lpWindowName, pTextStr))
				{
					return g_vWnd[i];
				}

			} /* end if NULL != lpClassName */

		} /* end for */

	} /* end if NULL != hWndRet */

	 return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*
 * SetWindowsHook() codes

#define WH_MIN              (-1)
#define WH_MSGFILTER        (-1)
#define WH_JOURNALRECORD    0
#define WH_JOURNALPLAYBACK  1
#define WH_KEYBOARD         2
#define WH_GETMESSAGE       3
#define WH_CALLWNDPROC      4
#define WH_CBT              5
#define WH_SYSMSGFILTER     6
#define WH_MOUSE            7
#define WH_HARDWARE         8
#define WH_DEBUG            9
#define WH_SHELL           10
#define WH_FOREGROUNDIDLE  11
#if(WINVER >= 0x0400)
#define WH_CALLWNDPROCRET  12
#endif /* WINVER >= 0x0400 

#if (_WIN32_WINNT >= 0x0400)
#define WH_KEYBOARD_LL     13
#define WH_MOUSE_LL        14
#endif // (_WIN32_WINNT >= 0x0400)
*/
HHOOK IcySetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId/*= NULL*/)
{
	return SetWindowsHookEx(idHook, lpfn, hmod, dwThreadId);
}
///////////////////////////////////////////////////////////////////////////////////////////////
HERROR IcyPostMsgToWndByClsName(LPCSTR lpClassName, UINT Msg, WPARAM wParam /* = NULL */, LPARAM lParam /* = NULL*/)
{
	HWND hWnd = NULL;
	BOOL bRet = TRUE;
	while (hWnd != IcyFindWindow(NULL, lpClassName))
	{   
		bRet = PostMessage(hWnd, Msg, wParam, lParam);
		if (!bRet)
		{
			ICY_ASSERT0(FALSE);
			return FAILE;
		}
		hWnd = NULL;
	}
	
	return SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////
HERROR IcyCloseWindowByClsName(LPCSTR lpClassName)
{
	return IcyPostMsgToWndByClsName(lpClassName, WM_CLOSE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
/* 向标签为lpWindowName的所有窗口发送消息 */
HERROR IcyPostMsgToWndByWinName(LPCSTR lpWindowName, UINT Msg, WPARAM wParam /* = NULL */, LPARAM lParam /* = NULL */)
{
	HWND hWnd = NULL;
	BOOL bRet = TRUE;
	while (hWnd != IcyFindWindow(lpWindowName, NULL))
	{   
		bRet = PostMessage(hWnd, Msg, wParam, lParam);
		if (!bRet)
		{
			ICY_ASSERT0(FALSE);
			return FAILE;
		}
		hWnd = NULL;// 
		Sleep(200);
	}

	return SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////

/* 关掉所有标签为 lpWindowName 的窗口 */
HERROR IcyCloseWindowByWinName(LPCSTR lpWindowName)
{
	return IcyPostMsgToWndByWinName(lpWindowName, WM_CLOSE);
}
///////////////////////////////////////////////////////////////////////////////////////////////

HERROR IcyCloseWindow(HWND hWnd)
{
	/* PostMessage,If the function succeeds, the return value is nonzero.
		If the function fails, the return value is zero.*/
	DWORD dwRet = PostMessage(hWnd, WM_CLOSE, 0, 0);

	return (dwRet ? SUCCESS : FAILE);
}
///////////////////////////////////////////////////////////////////////////////////////////////

HERROR IcySetOtherWindowText(HWND hWnd, LPCSTR lpText)
{
	return SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)lpText);
}
///////////////////////////////////////////////////////////////////////////////////////////////
HERROR IcyLeftClickBtn(HWND hWnd)
{
	SendMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
    SendMessage(hWnd, WM_LBUTTONUP, 0, 0);

	return SUCCESS;
}
HERROR IcyLeftDBClickBtn(HWND hWnd)
{
	SendMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
    SendMessage(hWnd, WM_LBUTTONUP, 0, 0);

	SendMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
    SendMessage(hWnd, WM_LBUTTONUP, 0, 0);
	return SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////
HERROR IcyEnumWindows(IN WND_VECTOR &wndVctor, IN HWND hwdParent /* = NULL */)
{
	if (INVALID_HANDLE_VALUE == hwdParent)
	{
		ICY_ASSERT1(FALSE, "Invalid handle!");
		return FAILE;
	}

	if (NULL == hwdParent) // 枚举所有父窗口
	{
		if (!EnumWindows(EnumWindowProc, (LPARAM)&wndVctor))
		{
			ICY_ASSERT2(FALSE, GetLastError());
			return FAILE;
		}
	}
	else // 枚举hwdParent的子窗口。
	{
		if (!EnumChildWindows(hwdParent, EnumWindowProc, (LPARAM)&wndVctor))
		{
			//ICY_ASSERT2(FALSE, GetLastError());
			return FAILE;
		}
	}

	return SUCCESS;
}
//======================================================================================//

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam)
{
	WND_VECTOR *pVctor = (WND_VECTOR *)lParam;
	pVctor->push_back(hwnd);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
