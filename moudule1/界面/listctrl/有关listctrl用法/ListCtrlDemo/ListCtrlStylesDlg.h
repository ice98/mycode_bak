#if !defined(AFX_LISTCTRLSTYLESDLG_H__D1102D05_14B4_4047_AB56_3FC12A8768B1__INCLUDED_)
#define AFX_LISTCTRLSTYLESDLG_H__D1102D05_14B4_4047_AB56_3FC12A8768B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlStylesDlg.h : header file
//
// Written by matt weagle (matt_weagle@hotmail.com)
// Copyright (c) 2000.
//
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// Consider yourself lucky if it works, unlucky if it doesn't.
//
// Thanks to Chris Maunder (chris@codeproject.com) for the 
// foregoing disclaimer.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////
// CListCtrlStylesDlg dialog

class CListCtrlStylesDlg : public CDialog
{
// Construction
public:
	CListCtrlStylesDlg(CListCtrl *pListCtrl, CWnd* pParent = NULL);   // custom constructor
	
	const DWORD&	GetStyles() {return m_dwStyles;}
// Dialog Data
	//{{AFX_DATA(CListCtrlStylesDlg)
	enum { IDD = IDD_STYLES };
	CListCtrl	m_cListStyles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlStylesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddStyleItem(LPCTSTR lpszItem, BOOL bState, DWORD dwData);
	CListCtrl *m_pListCtrl;
	DWORD		m_dwStyles;

	// Generated message map functions
	//{{AFX_MSG(CListCtrlStylesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnInfoTip( NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLSTYLESDLG_H__D1102D05_14B4_4047_AB56_3FC12A8768B1__INCLUDED_)
