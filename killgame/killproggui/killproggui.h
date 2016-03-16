// killproggui.h : main header file for the KILLPROGGUI application
//

#if !defined(AFX_KILLPROGGUI_H__1CA35545_9E9F_454B_A78C_8C26C2EF1AB5__INCLUDED_)
#define AFX_KILLPROGGUI_H__1CA35545_9E9F_454B_A78C_8C26C2EF1AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKillprogguiApp:
// See killproggui.cpp for the implementation of this class
//

class CKillprogguiApp : public CWinApp
{
public:
	CKillprogguiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillprogguiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKillprogguiApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLPROGGUI_H__1CA35545_9E9F_454B_A78C_8C26C2EF1AB5__INCLUDED_)
