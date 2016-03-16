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

/* ȡ��ǩ���ɹ�����ȡ�õ��ֽ�����ʧ�ܷ���null */
INT IcyGetWindowText(HWND hWnd, PSTR strBuf, DWORD dwMaxRcv = 255 );

/*�������Ϊ����дnil,���ڲ���һ����FORM��Ҳ������BUTTON��EDIT��LABEL��*/
HWND IcyFindWindow(LPCSTR lpWindowName, LPCSTR lpClassName = NULL);

/* ���ùҹ� */
HHOOK IcySetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId = 0);

/* ����������ΪlpClassName�Ĵ��ڷ�����Ϣ
IEFrame,Edit,Dialog,SysListView32,Static,ComboLBox
...*/
HERROR IcyPostMsgToWndByClsName(PCSTR lpClassName, UINT Msg, WPARAM wParam = NULL, LPARAM lParam = NULL);

/* �ص���������ΪlpClassName�Ĵ��� */
HERROR IcyCloseWindowByClsName(LPCSTR lpClassName);

/* ���ǩΪlpWindowName�����д��ڷ�����Ϣ */
HERROR IcyPostMsgToWndByWinName(LPCSTR lpWindowName, UINT Msg, WPARAM wParam = NULL, LPARAM lParam = NULL);

/* �ص����б�ǩΪlpClassName�Ĵ��� */
HERROR IcyCloseWindowByWinName(LPCSTR lpClassName);

/* �ص�hwnd��ǵĴ��� */
HERROR IcyCloseWindow(HWND hWnd);

/* ������һ���ڵ��ı� */
HERROR IcySetOtherWindowText(HWND hWnd, LPCSTR lpText);

/* ģ������ť */
HERROR IcyLeftClickBtn(HWND hWnd);

/* ģ��˫����� */
HERROR IcyLeftDBClickBtn(HWND hWnd);


/* ���hwdParent == NULL,���ǲ��Ҷ������д��ڣ�
�����ΪNULL�����ǲ���ĳһ�����ڵ������Ӵ��ھ����
����vector�ڡ�
*/
HERROR IcyEnumWindows(IN WND_VECTOR &wndVctor, IN HWND hwdParent = NULL);

/* ����ö�ٻص����� */
BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam); 

#endif