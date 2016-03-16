/*****************************************************************************\
*                                                                             *
* IceWnd.h -  Functions refered to forms and windows						  *
*                                                                             *
* Version 1.0                                                                 *
*                                                                             *
* NOTE:																		  *
*                                                                             *
* Copyright 2008 - 2012 Ice.   All rights reserved.							  *
*                                                                             *
\*****************************************************************************/
#ifndef _ICE_WND_H
#define _ICE_WND_H
#include "Ice.h"

/* 取标签，成功返回取得的字节数，失败返回null */
INT IcyGetWindowText(HWND hWnd, PSTR strBuf, DWORD dwMaxRcv = 255 );

/*如果标题为空则写nil,窗口并不一定是FORM，也可能是BUTTON，EDIT，LABEL。*/
HWND IcyFindWindow(LPCSTR lpWindowName, LPCSTR lpClassName = NULL);

/* 设置挂钩 */
HHOOK IcySetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId = 0);

/* 向所有类名为lpClassName的窗口发送消息
IEFrame,Edit,Dialog,SysListView32,Static,ComboLBox
...*/
HERROR IcyPostMsgToWndByClsName(PCSTR lpClassName, UINT Msg, WPARAM wParam = NULL, LPARAM lParam = NULL);

/* 关掉所有类名为lpClassName的窗口 */
HERROR IcyCloseWindowByClsName(LPCSTR lpClassName);

/* 向标签为lpWindowName的所有窗口发送消息 */
HERROR IcyPostMsgToWndByWinName(LPCSTR lpWindowName, UINT Msg, WPARAM wParam = NULL, LPARAM lParam = NULL);

/* 关掉所有标签为lpClassName的窗口 */
HERROR IcyCloseWindowByWinName(LPCSTR lpClassName);

/* 关掉hwnd标记的窗口 */
HERROR IcyCloseWindow(HWND hWnd);

/* 设置另一窗口的文本 */
HERROR IcySetOtherWindowText(HWND hWnd, LPCSTR lpText);

/* 模拟点击按钮 */
HERROR IcyLeftClickBtn(HWND hWnd);

/* 模拟双击左键 */
HERROR IcyLeftDBClickBtn(HWND hWnd);


/* 如果hwdParent == NULL,则是查找顶层所有窗口，
如果不为NULL，则是查找某一父窗口的所有子窗口句柄，
放入vector内。
*/
HERROR IcyEnumWindows(IN WND_VECTOR &wndVctor, IN HWND hwdParent = NULL);

/* 窗口枚举回调函数 */
BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam); 

#endif