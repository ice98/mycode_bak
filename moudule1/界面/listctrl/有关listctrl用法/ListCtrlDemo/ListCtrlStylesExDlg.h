#if !defined(AFX_LISTCTRLSTYLESEXDLG_H__144E47F8_F157_427C_892E_C3EEFD93CCA9__INCLUDED_)
#define AFX_LISTCTRLSTYLESEXDLG_H__144E47F8_F157_427C_892E_C3EEFD93CCA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlStylesExDlg.h : header file
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
// CListCtrlStylesExDlg dialog

#include "Hyperlink.h"

class CListCtrlStylesExDlg : public CDialog
{
// Construction
public:
	CListCtrlStylesExDlg(CListCtrl *pListCtrl = NULL, CWnd* pParent = NULL);   // custom constructor
	
	const DWORD&	GetStyles() {return m_dwStyles;}
	const BOOL&		GetUseHotCursor() {	return m_bUseHotCursor;}
	void			SetUseHotCursor(const BOOL& bUseHot) {m_bUseHotCursor = bUseHot;}


// Dialog Data
	//{{AFX_DATA(CListCtrlStylesExDlg)
	enum { IDD = IDD_STYLES_EX };
	CHyperLink	m_cVersionInfo;
	CListCtrl	m_cListStylesEx;
	BOOL	m_bUseHotCursor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlStylesExDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableHotCursor();
	void AddStyleItem(LPCTSTR lpszItem, BOOL bState, DWORD dwData);
	DWORD		m_dwStyles;
	CListCtrl *m_pListCtrl;

	// Generated message map functions
	//{{AFX_MSG(CListCtrlStylesExDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedListStylesEx(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnInfoTip( NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLSTYLESEXDLG_H__144E47F8_F157_427C_892E_C3EEFD93CCA9__INCLUDED_)
