// KeyBoardApp.h : main header file for the KEYBOARDAPP application
//

#if !defined(AFX_KEYBOARDAPP_H__171BC31E_26F9_4D65_8036_C6248FFDCCC3__INCLUDED_)
#define AFX_KEYBOARDAPP_H__171BC31E_26F9_4D65_8036_C6248FFDCCC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppApp:
// See KeyBoardApp.cpp for the implementation of this class
//

class CKeyBoardAppApp : public CWinApp
{
public:
	CKeyBoardAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardAppApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyBoardAppApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDAPP_H__171BC31E_26F9_4D65_8036_C6248FFDCCC3__INCLUDED_)
