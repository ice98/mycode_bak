// ListCtrlStylesDlg.cpp : implementation file
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
#include "ListCtrlStylesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlStylesDlg dialog


CListCtrlStylesDlg::CListCtrlStylesDlg(CListCtrl *pListCtrl /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlStylesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCtrlStylesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pListCtrl);
	m_pListCtrl = pListCtrl;

}


void CListCtrlStylesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlStylesDlg)
	DDX_Control(pDX, IDC_LIST_STYLES, m_cListStyles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListCtrlStylesDlg, CDialog)
	//{{AFX_MSG_MAP(CListCtrlStylesDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY( LVN_GETINFOTIP, IDC_LIST_STYLES, OnInfoTip )
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CListCtrlStylesDlg message handlers

BOOL CListCtrlStylesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Setup the list control
	CRect rect;
	m_cListStyles.GetClientRect(&rect);
	m_cListStyles.InsertColumn(0, _T("Styles"), LVCFMT_LEFT, rect.Width());

	ListView_SetExtendedListViewStyle(m_cListStyles.m_hWnd, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	
	
	// Setup the list control
	DWORD dwStyles = m_pListCtrl->GetStyle();

	AddStyleItem( _T("LVS_ALIGNLEFT"), (dwStyles & LVS_ALIGNLEFT) != 0, LVS_ALIGNLEFT);
	AddStyleItem( _T("LVS_ALIGNTOP"), (dwStyles & LVS_ALIGNTOP) != 0, LVS_ALIGNTOP);
	AddStyleItem( _T("LVS_AUTOARRANGE"), (dwStyles & LVS_AUTOARRANGE) != 0, LVS_AUTOARRANGE);
	AddStyleItem( _T("LVS_NOCOLUMNHEADER"), (dwStyles & LVS_NOCOLUMNHEADER) != 0, LVS_NOCOLUMNHEADER);
	AddStyleItem( _T("LVS_NOLABELWRAP"), (dwStyles & LVS_NOLABELWRAP) != 0, LVS_NOLABELWRAP);
	AddStyleItem( _T("LVS_NOSCROLL"), (dwStyles & LVS_NOSCROLL) != 0, LVS_NOSCROLL);
	AddStyleItem( _T("LVS_NOSORTHEADER"), (dwStyles & LVS_NOSORTHEADER) != 0, LVS_NOSORTHEADER);
	AddStyleItem( _T("LVS_SHOWSELALWAYS"), (dwStyles & LVS_SHOWSELALWAYS) != 0, LVS_SHOWSELALWAYS);
	AddStyleItem( _T("LVS_SINGLESEL"), (dwStyles & LVS_SINGLESEL) != 0, LVS_SINGLESEL);
	AddStyleItem( _T("LVS_SORTASCENDING"), (dwStyles & LVS_SORTASCENDING) != 0, LVS_SORTASCENDING);
	AddStyleItem( _T("LVS_SORTDESCENDING"), (dwStyles & LVS_SORTDESCENDING) != 0, LVS_SORTDESCENDING);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnOK

	Slightly different than CListCtrlStylesDlgEx's handling of the updated
	style bits.  Whereas that class dynamically tracks the style bits, this
	one just waits until the user is done.  Then we run through the 
	entire list control items and create the new styles that should be applied
	to the list control back in the main dialog.
  
	Params
		None
  
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlStylesDlg::OnOK() 
{
	// Loop through the list control and get all the styles that are selected.  We
	// make this easier by storing the style value as the data member of each item.
	m_dwStyles = m_pListCtrl->GetStyle();
	int nNumItems = m_cListStyles.GetItemCount();
	for (int i = 0; i < nNumItems; i++)
	{
		if (ListView_GetCheckState(m_cListStyles.GetSafeHwnd(), i))
			m_dwStyles |= m_cListStyles.GetItemData(i);
		else	
			m_dwStyles &= ~(m_cListStyles.GetItemData(i));

	}

	CDialog::OnOK();
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
void CListCtrlStylesDlg::AddStyleItem(LPCTSTR lpszItem, BOOL bState, DWORD dwData)
{
	int nPos = max(m_cListStyles.GetItemCount(),0);
	nPos = m_cListStyles.InsertItem(nPos, lpszItem);
	m_cListStyles.SetItemData(nPos, dwData);

	// Use macro to set check state
	ListView_SetItemState(m_cListStyles.m_hWnd, nPos, UINT((int(bState) + 1) << 12), LVIS_STATEIMAGEMASK);

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
void CListCtrlStylesDlg::OnInfoTip( NMHDR * pNMHDR, LRESULT * pResult )
{
	NMLVGETINFOTIP*	pInfoTip = reinterpret_cast<NMLVGETINFOTIP*>(pNMHDR);
	ASSERT(pInfoTip);

	// Get the item data for this unit
	DWORD dwData = m_cListStyles.GetItemData(pInfoTip->iItem);
	

	// Based on the flag value, show a tool tip with the information about this style bit
	CString strText;
	switch (dwData)
	{
	case LVS_ALIGNLEFT:
		strText = _T("Specifies that items are left-aligned in icon and small icon view."); break;
	case LVS_ALIGNTOP:
		strText = _T("Specifies that items are aligned with the top of the control in icon and small icon view."); break;
	case LVS_AUTOARRANGE:
		strText = _T("Specifies that icons are automatically kept arranged in icon view and small icon view."); break;
	case LVS_NOCOLUMNHEADER:
		strText = _T("Specifies that a column header is not displayed in report view."); break;
	case LVS_NOLABELWRAP:
		strText = _T("Displays item text on a single line in icon view."); break;
	case LVS_NOSCROLL:
		strText = _T("Disables scrolling. All items must be within the client area.");break;
	case LVS_NOSORTHEADER:
		strText = _T("Specifies that column headers do not work like buttons.");break;
	case LVS_SHOWSELALWAYS:
		strText = _T("Always show the selection, if any, even if the control does not have the focus."); break;
	case LVS_SINGLESEL:
		strText = _T("Allows only one item at a time to be selected."); break;
	case LVS_SORTASCENDING:
		strText = _T("Sorts items based on item text in ascending order."); break;
	case LVS_SORTDESCENDING:
		strText = _T("Sorts items based on item text in descending order."); break;	
	default:
		strText = _T("Unknown style option."); break;

	}
	_tcscpy(pInfoTip->pszText, (LPCTSTR)strText);

}

