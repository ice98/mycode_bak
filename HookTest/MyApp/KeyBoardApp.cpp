// KeyBoardApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KeyBoardApp.h"
#include "KeyBoardAppDlg.h"

#include "MyDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppApp

BEGIN_MESSAGE_MAP(CKeyBoardAppApp, CWinApp)
	//{{AFX_MSG_MAP(CKeyBoardAppApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppApp construction

CKeyBoardAppApp::CKeyBoardAppApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKeyBoardAppApp object

CKeyBoardAppApp theApp;

 HHOOK g_hHook= NULL;

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppApp initialization

#if 0
template<typename T> 
DWORD GetFuncAddr(T t)
{
    DWORD dwResutl = 0;
    __asm
    {
        mov eax, [ebp+8];
        mov [dwResutl], eax;
    }

    return dwResutl;
}

#endif

template<typename ToType, typename FromType> 
void GetFuncAddr(ToType &addr, FromType f)
{
    union
    {
        ToType _t;
        FromType _f;
    }ut;

    ut._f = f;
    addr =  ut._t;     
}


void GetFuncAddr2(DWORD &addr, DWORD f)
{
    __asm
    {
        mov eax, f
        mov [addr],eax
    }
}


BYTE abyBuffer[1024] = {0};

 unsigned int getit(BYTE *byBuffer, WORD wLen)
 {
     //DWORD dwAddrMsgBox = (DWORD)GetProcAddress(0, "CKeyBoardAppApp::ExitInstance");

 //     DWORD dwAddrMsgBox = (DWORD)(&CKeyBoardAppApp::InitInstance);
//      if (0 == dwAddrMsgBox)
//      {
//          AfxMessageBox("dwAddrMsgBox = NULL");
//      }

        //void *pt = (VOID*)&AfxMessageBox;  
        //DWORD dwAddrMsgBox = pt;
        DWORD dwAddrMsgBox = 0;
        GetFuncAddr2(dwAddrMsgBox, (DWORD)&AfxMessageBox);

        char *p = "I love you";

        __asm
        {
            push -1;
            push MB_OK;
            push p;
            call [dwAddrMsgBox] 
        }

    return 0;
 }




    
    
BOOL CKeyBoardAppApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    getit(abyBuffer, 1024);
        

    
	if (!InstallHook("L:\\mycode\\HookTest\\MyApp\\keylog.txt"))
		AfxMessageBox("Install Hook failed!");

	CKeyBoardAppDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CKeyBoardAppApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

    UnHook();
	return CWinApp::ExitInstance();
}


