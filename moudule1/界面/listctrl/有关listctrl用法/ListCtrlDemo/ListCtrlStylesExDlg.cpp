// ListCtrlStylesExDlg.cpp : implementation file
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
#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ListCtrlStylesExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlStylesExDlg dialog


CListCtrlStylesExDlg::CListCtrlStylesExDlg(CListCtrl *pListCtrl /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlStylesExDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCtrlStylesExDlg)
	m_bUseHotCursor = FALSE;
	//}}AFX_DATA_INIT
	ASSERT(pListCtrl);
	m_pListCtrl = pListCtrl;
}


void CListCtrlStylesExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlStylesExDlg)
	DDX_Control(pDX, IDC_VERSION_INFO, m_cVersionInfo);
	DDX_Control(pDX, IDC_LIST_STYLES_EX, m_cListStylesEx);
	DDX_Check(pDX, IDC_USE_HOTCURSOR, m_bUseHotCursor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListCtrlStylesExDlg, CDialog)
	//{{AFX_MSG_MAP(CListCtrlStylesExDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STYLES_EX, OnItemchangedListStylesEx)
	//}}AFX_MSG_MAP
	ON_NOTIFY( LVN_GETINFOTIP, IDC_LIST_STYLES_EX, OnInfoTip )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlStylesExDlg message handlers

BOOL CListCtrlStylesExDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Setup the list control
	CRect rect;
	m_cListStylesEx.GetClientRect(&rect);
	m_cListStylesEx.InsertColumn(0, _T("Extended Styles"), LVCFMT_LEFT, rect.Width());
	ListView_SetExtendedListViewStyle(m_cListStylesEx.m_hWnd, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	
	// Setup the list control
	m_dwStyles = m_pListCtrl->GetExtendedStyle();


	AddStyleItem( _T("LVS_EX_CHECKBOXES"), (m_dwStyles & LVS_EX_CHECKBOXES) != 0, LVS_EX_CHECKBOXES);
	AddStyleItem( _T("LVS_EX_FLATSB"), (m_dwStyles & LVS_EX_FLATSB) != 0, LVS_EX_FLATSB);
	AddStyleItem( _T("LVS_EX_FULLROWSELECT"), (m_dwStyles & LVS_EX_FULLROWSELECT) != 0, LVS_EX_FULLROWSELECT);
	AddStyleItem( _T("LVS_EX_GRIDLINES"), (m_dwStyles & LVS_EX_GRIDLINES) != 0, LVS_EX_GRIDLINES);
	AddStyleItem( _T("LVS_EX_HEADERDRAGDROP"), (m_dwStyles & LVS_EX_HEADERDRAGDROP) != 0, LVS_EX_HEADERDRAGDROP);
	AddStyleItem( _T("LVS_EX_INFOTIP"), (m_dwStyles & LVS_EX_INFOTIP) != 0, LVS_EX_INFOTIP);
	AddStyleItem( _T("LVS_EX_ONECLICKACTIVATE"), (m_dwStyles & LVS_EX_ONECLICKACTIVATE) != 0, LVS_EX_ONECLICKACTIVATE);
	AddStyleItem( _T("LVS_EX_SUBITEMIMAGES"), (m_dwStyles & LVS_EX_SUBITEMIMAGES) != 0, LVS_EX_SUBITEMIMAGES);
	AddStyleItem( _T("LVS_EX_TRACKSELECT"), (m_dwStyles & LVS_EX_TRACKSELECT) != 0, LVS_EX_TRACKSELECT);
	AddStyleItem( _T("LVS_EX_TWOCLICKACTIVATE"), (m_dwStyles & LVS_EX_TWOCLICKACTIVATE) != 0, LVS_EX_TWOCLICKACTIVATE);
	AddStyleItem( _T("LVS_EX_UNDERLINECOLD"), (m_dwStyles & LVS_EX_UNDERLINECOLD) != 0, LVS_EX_UNDERLINECOLD);
	AddStyleItem( _T("LVS_EX_UNDERLINEHOT"), (m_dwStyles & LVS_EX_UNDERLINEHOT) != 0, LVS_EX_UNDERLINEHOT);

	// Setup the URL
	m_cVersionInfo.SetURL(_T("http://msdn.microsoft.com/library/psdk/shellcc/shell/Versions.htm"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
/*
	AddStyleItem

	Helper function to insert the items into the list control.  Shows
	how to use CListCtrl::InsertItem without using the LVITEM 
	struct.
  
	Params
		lpszItem	Item text
		bState		Should the item have a check mark?
		dwData		Data to associate with item
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlStylesExDlg::AddStyleItem(LPCTSTR lpszItem, BOOL bState, DWORD dwData)
{
	int nPos = max(m_cListStylesEx.GetItemCount(),0);
	nPos = m_cListStylesEx.InsertItem(nPos, lpszItem);
	m_cListStylesEx.SetItemData(nPos, dwData);

	// Use macro to set check state
	ListView_SetItemState (m_cListStylesEx.m_hWnd, nPos, UINT((int(bState) + 1) << 12), LVIS_STATEIMAGEMASK);

}


/////////////////////////////////////////////////////////////////////////////
/*
	OnInfoTip

	This is a hand-entered handler for the LVN_GETINFOTIP WM_NOTIFY message
	which we enabled by including LVS_EX_INFOTIP in the extended style
	bits of the list control.  When this style bit is enabled, it sends 
	the parent window a LVN_GETINFOTIP message with a pointer to a 
	LVGETINFOTIP struct.  We populate the pszParam of this struct with 
	documentation for the item selected.  A more robust implementation might 
	associate the data member of each list control item with a string 
	allocated on the heap or some user defined struct.
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlStylesExDlg::OnInfoTip( NMHDR * pNMHDR, LRESULT * pResult )
{
	NMLVGETINFOTIP*	pInfoTip = reinterpret_cast<NMLVGETINFOTIP*>(pNMHDR);
	ASSERT(pInfoTip);

	// Get the item data for this unit
	DWORD dwData = m_cListStylesEx.GetItemData(pInfoTip->iItem);
	
	// Addition strings for support of version information
	CString strVer47, strVer471;
	strVer47 = _T("  Supported in version 4.70+.");
	strVer471 = _T("  Supported in version 4.71+.");


	// Based on the flag value, show a tool tip with the information about this style bit
	CString strText;
	switch (dwData)
	{
	case LVS_EX_CHECKBOXES:
		strText = _T("Enables check boxes for items in a list view control."); strText += strVer47; break;
	case LVS_EX_FLATSB:
		strText = _T("Enables flat scroll bars in the list view. "); strText += strVer47;break;
	case LVS_EX_FULLROWSELECT:
		strText = _T("When an item is selected, the item and all its subitems are highlighted. This style is available only in conjunction with the LVS_REPORT style."); strText += strVer47;break;
	case LVS_EX_GRIDLINES:
		strText = _T("Displays gridlines around items and subitems. This style is available only in conjunction with the LVS_REPORT style."); strText += strVer47;break;
	case LVS_EX_HEADERDRAGDROP:
		strText = _T("Enables drag-and-drop reordering of columns in a list view control. This style is only available to list view controls that use the LVS_REPORT style."); strText += strVer47;break;
	case LVS_EX_INFOTIP:
		strText = _T("Enables a LVN_GETINFOTIP notification message to the parent window before displaying an item's tooltip."); strText += strVer471; break;
	case LVS_EX_ONECLICKACTIVATE:
		strText = _T("The list view control sends an LVN_ITEMACTIVATE notification message to the parent window when the user clicks an item.");strText += strVer47; break;
	case LVS_EX_SUBITEMIMAGES:
		strText = _T("Allows images to be displayed for subitems."); strText += strVer47;break;
	case LVS_EX_TRACKSELECT:
		strText = _T("Enables hot-track selection in a list view control.  Requires either LVS_EX_ONECLICKACTIVATE or LVS_EX_TWOCLICKACTIVATE."); strText += strVer47; break;
	case LVS_EX_TWOCLICKACTIVATE:
		strText = _T("The list view control sends an LVN_ITEMACTIVATE notification message to the parent window when the user double-clicks an item."); strText += strVer47; break;
	case LVS_EX_UNDERLINECOLD:
		strText = _T("Causes non-hot items that are activatable to be displayed with underlined text. This style requires that LVS_EX_TWOCLICKACTIVATE also be set."); strText += strVer471; break;	
	case LVS_EX_UNDERLINEHOT:
		strText = _T("Causes hot items that are activatable to be displayed with underlined text. This style requires that LVS_EX_ONECLICKACTIVATE or LVS_EX_TWOCLICKACTIVATE also be set."); strText += strVer471;break;	
	default:
		strText = _T("Unknown style option."); break;

	}


	// Copy it in the allocated buffer
	_tcsncpy(pInfoTip->pszText, (LPCTSTR)strText, pInfoTip->cchTextMax);

}

/////////////////////////////////////////////////////////////////////////////
/*
	EnableHotCursor

	Depending on what style bits are selected in the list control, toggle
	the state of the "Use custom hot cursor" item.  It is called each time
	through the OnItemchangedListStylesEx action.
  
	Params
		none
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlStylesExDlg::EnableHotCursor()
{
	// If the styles are setup to allow hot tracking, then enable the hot cursor window
	if (LVS_EX_TRACKSELECT & m_dwStyles)
	{
		CWnd *pWnd = GetDlgItem(IDC_USE_HOTCURSOR);
		ASSERT(pWnd);
		if ((LVS_EX_ONECLICKACTIVATE & m_dwStyles) || (LVS_EX_TWOCLICKACTIVATE & m_dwStyles))
			pWnd->EnableWindow(TRUE);
		else
			pWnd->EnableWindow(FALSE);
	}

}


/////////////////////////////////////////////////////////////////////////////
/*
	OnItemchangedListStylesEx

	When the state of an item in the list control has changed, we need to 
	update the local copy of the style bits to apply to the list control
	window.  Also, depending on what items are checked in the list control,
	we pass control to EnableHotCursor() which is responsible for toggling
	the state of the dialog window.
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlStylesExDlg::OnItemchangedListStylesEx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	BOOL bChecked = m_cListStylesEx.GetCheck(pNMListView->iItem);
	if (bChecked)
		m_dwStyles |= (pNMListView->lParam);
	else
		m_dwStyles &= ~(pNMListView->lParam);


	// Toggle the hot cursor window
	EnableHotCursor();

	*pResult = 0;
}
