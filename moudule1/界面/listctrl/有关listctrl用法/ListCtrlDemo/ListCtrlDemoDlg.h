// ListCtrlDemoDlg.h : header file
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
#if !defined(AFX_LISTCTRLDEMODLG_H__75B0722D_DB58_45BC_83FB_2E56F8C8BDC8__INCLUDED_)
#define AFX_LISTCTRLDEMODLG_H__75B0722D_DB58_45BC_83FB_2E56F8C8BDC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HistoryEdit.h"
#include "resource.h"
#include "ListCtrlStylesDlg.h"
#include "ListCtrlStylesExDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog

class CListCtrlDemoDlg : public CDialog
{
// Construction
public:
	CListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CListCtrlDemoDlg)
	enum { IDD = IDD_LISTCTRLDEMO_DIALOG };
	CHistoryEdit	m_Log;
	CListCtrl	m_cListCtrl;
	CTabCtrl	m_cTabListMode;
	int		m_nItems;
	int		m_nSelectItem;
	BOOL	m_bImage;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableBkImage(BOOL bEnable);
	void RecreateListControl(const DWORD &dwStyles, const DWORD &dwStylesEx);

	void InitListCtrlCols();
	void InsertItems();
	void InitTabCtrl();
	BOOL InitImageList();

	BOOL	m_bHotCursor;
	HCURSOR m_hHotCursor;
	HCURSOR m_hCustomHotCursor;
	
	
	
	HICON m_hIcon;
	CImageList	m_cImageListNormal, m_cImageListSmall, m_cImageListState;
	
	// Generated message map functions
	//{{AFX_MSG(CListCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeListCtrlMode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateItemCount();
	afx_msg void OnStylesEx();
	afx_msg void OnStyles();
	afx_msg void OnGetSelected();
	afx_msg void OnItemSelect();
	afx_msg void OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClearLog();
	afx_msg void OnBkImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLDEMODLG_H__75B0722D_DB58_45BC_83FB_2E56F8C8BDC8__INCLUDED_)
