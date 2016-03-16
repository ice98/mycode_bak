// module2.h : main header file for the MODULE2 application
//

#if !defined(AFX_MODULE2_H__F092F7F3_9F4F_433B_B95F_34886E90A75F__INCLUDED_)
#define AFX_MODULE2_H__F092F7F3_9F4F_433B_B95F_34886E90A75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CModule2App:
// See module2.cpp for the implementation of this class
//

class CModule2App : public CWinApp
{
public:
	CModule2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModule2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CModule2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULE2_H__F092F7F3_9F4F_433B_B95F_34886E90A75F__INCLUDED_)
