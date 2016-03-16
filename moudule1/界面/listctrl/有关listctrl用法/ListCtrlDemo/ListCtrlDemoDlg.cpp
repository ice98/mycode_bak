// ListCtrlDemoDlg.cpp : implementation file
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
#include "ListCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Hyperlink.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_cEmail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EMAIL, m_cEmail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog

CListCtrlDemoDlg::CListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCtrlDemoDlg)
	m_nItems = 100;
	m_nSelectItem = 0;
	m_bImage = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlDemoDlg)
	DDX_Control(pDX, IDC_LOG, m_Log);
	DDX_Control(pDX, IDC_LIST_CTRL, m_cListCtrl);
	DDX_Control(pDX, IDC_LIST_CTRL_MODE, m_cTabListMode);
	DDX_Text(pDX, IDC_LIST_CTRL_NUMITEMS, m_nItems);
	DDV_MinMaxInt(pDX, m_nItems, 0, 32257);
	DDX_Text(pDX, IDC_LIST_CTRL_SELECTITEM, m_nSelectItem);
	DDV_MinMaxInt(pDX, m_nSelectItem, 0, 32567);
	DDX_Check(pDX, IDC_BK_IMAGE, m_bImage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListCtrlDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CListCtrlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_LIST_CTRL_MODE, OnSelchangeListCtrlMode)
	ON_BN_CLICKED(IDC_UPDATE_ITEM_COUNT, OnUpdateItemCount)
	ON_BN_CLICKED(IDC_STYLES_EX, OnStylesEx)
	ON_BN_CLICKED(IDC_STYLES, OnStyles)
	ON_BN_CLICKED(IDC_GET_SELECTED, OnGetSelected)
	ON_BN_CLICKED(IDC_ITEM_SELECT, OnItemSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CTRL, OnItemchangedListCtrl)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CTRL, OnRclickListCtrl)
	ON_BN_CLICKED(IDC_CLEAR_LOG, OnClearLog)
	ON_BN_CLICKED(IDC_BK_IMAGE, OnBkImage)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg message handlers

BOOL CListCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	// Initial extended style for the list control on this dialog
	DWORD dwStyle = m_cListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_cListCtrl.SetExtendedStyle(dwStyle);

	// initialize the standard and custom hot cursors
	m_hCustomHotCursor= AfxGetApp()->LoadCursor(IDC_HOTCURSOR);
	m_hHotCursor= m_cListCtrl.GetHotCursor();
	m_bHotCursor = FALSE;

	// Setup the tab header
	InitTabCtrl();

	// Setup the column headings
	InitListCtrlCols();

	// Create the image list that is attached to the list control
	InitImageList();

	// Insert the default dummy items
	InsertItems();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListCtrlDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListCtrlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
/*
	InitImageList

	Create the image list that is associated with the list control.  Then 
	attach the list of images to the control so that we can add items with
	images.
	
	Params
		None

	Returns 
		BOOL	Image list created and attached?
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CListCtrlDemoDlg::InitImageList()
{
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(32,32, ILC_COLOR8 |ILC_MASK , 8, 1);
	m_cImageListNormal.Attach(hList);

	hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_cImageListSmall.Attach(hList);


	// Load the large icons
	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_IMAGES_NORMAL);
	m_cImageListNormal.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();

	// Load the small icons
	cBmp.LoadBitmap(IDB_IMAGES_SMALL);
	m_cImageListSmall.Add(&cBmp, RGB(255,0, 255));

	// Attach them
	m_cListCtrl.SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	m_cListCtrl.SetImageList(&m_cImageListSmall, LVSIL_SMALL);

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
/*
	InitTabCtrl

	Setup the tab control with the different modes for the list control.
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::InitTabCtrl()
{

	m_cTabListMode.InsertItem(0, _T("Icon"));
	m_cTabListMode.InsertItem(1, _T("Small Icon"));
	m_cTabListMode.InsertItem(2, _T("List"));
	m_cTabListMode.InsertItem(3, _T("Details"));
}


/////////////////////////////////////////////////////////////////////////////
/*
	OnSelchangeListCtrlMode

	Whent the user changes the mode of the list control, we need to update
	the list control styles and then insert the new items.
	
	Params
		(standard)

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnSelchangeListCtrlMode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the current data and don't redraw while we decide what to do.
	UpdateData(TRUE);
//	m_cListCtrl.SetRedraw(FALSE);

	// Remove whatever style is there currently
	m_cListCtrl.ModifyStyle(LVS_ICON | LVS_LIST | LVS_REPORT | LVS_SMALLICON ,0);

	// Switch the tab mode
	switch (m_cTabListMode.GetCurSel())
	{
	case 0: // Icon mode
		m_cListCtrl.ModifyStyle(0, LVS_ICON);
		break;
	case 1:	// Small icons
		m_cListCtrl.ModifyStyle(0, LVS_SMALLICON);
		break;
	case 2: // List mode
		m_cListCtrl.ModifyStyle(0, LVS_LIST);
		break;
	case 3:	// Details mode
		m_cListCtrl.ModifyStyle(0, LVS_REPORT);

		break;
	}
	// Fill the list control
	InsertItems();

//	m_cListCtrl.SetRedraw(TRUE);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/*
	InsertItems

	Insert the items into the list control.   
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::InsertItems()
{
	// Delete the current contents
	m_cListCtrl.DeleteAllItems();

	// Use the LV_ITEM structure to insert the items
	LVITEM lvi;
	CString strItem;
	for (int i = 0; i < m_nItems; i++)
	{
		// Insert the first item
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
		strItem.Format(_T("Item %i"), i);
	
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		lvi.iImage = i%8;		// There are 8 images in the image list
		m_cListCtrl.InsertItem(&lvi);

		// Set subitem 1
		strItem.Format(_T("%d"), 10*i);
		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_cListCtrl.SetItem(&lvi);

		// Set subitem 2
		strItem.Format(_T("%s"), COleDateTime::GetCurrentTime().Format(_T("Created: %I:%M:%S %p, %m/%d/%Y")));
		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_cListCtrl.SetItem(&lvi);

	}
}

/////////////////////////////////////////////////////////////////////////////
/*
	InitListCtrlCols

	This is called when we first create the list control as well as part of the
	RecreateListControl(...) call.  The reason is that when we destroy
	and then create the list control in response to changing list control
	styles we lose the column information.
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::InitListCtrlCols()
{
	// Insert some columns
	CRect rect;
	m_cListCtrl.GetClientRect(&rect);
	int nColInterval = rect.Width()/5;

	m_cListCtrl.InsertColumn(0, _T("Item Name"), LVCFMT_LEFT, nColInterval*3);
	m_cListCtrl.InsertColumn(1, _T("Value"), LVCFMT_LEFT, nColInterval);
	m_cListCtrl.InsertColumn(2, _T("Time"), LVCFMT_LEFT, rect.Width()-4*nColInterval);

}

/////////////////////////////////////////////////////////////////////////////
/*
	OnUpdateItemCount

	When the user presses the refresh button, we simply redirect control
	to the Tab handler.
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnUpdateItemCount() 
{
	// TODO: Add your control notification handler code here
	NMHDR nmhdr;
	LRESULT lResult;
	OnSelchangeListCtrlMode(&nmhdr, &lResult);

}

/////////////////////////////////////////////////////////////////////////////
/*
	OnStylesEx

	Handler for the Extended Styles button.  We show the dialog from which
	the user can view and change the extended styles available to the list
	control.
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnStylesEx() 
{
	CListCtrlStylesExDlg dlg(&m_cListCtrl);
	dlg.SetUseHotCursor(m_bHotCursor);

	if (dlg.DoModal() != IDOK)
		return;

	// Just recreate the control with the new styles, since some styles can't be
	// dynamically reset
	RecreateListControl(m_cListCtrl.GetStyle(), dlg.GetStyles());
	m_bHotCursor = dlg.GetUseHotCursor();
	if (m_bHotCursor)
		m_cListCtrl.SetHotCursor(m_hCustomHotCursor);
	else
		m_cListCtrl.SetHotCursor(m_hHotCursor);

}


/////////////////////////////////////////////////////////////////////////////
/*
	OnStyles

	Handler for the Styles button.  We show the dialog from which
	the user can view and change the styles available to the list
	control.
	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnStyles() 
{
	// TODO: Add your control notification handler code here
	CListCtrlStylesDlg dlg(&m_cListCtrl);
	if (dlg.DoModal() != IDOK)
		return;
	

	// Just recreate the control with the new styles, since some styles can't be
	// dynamically reset
	RecreateListControl(dlg.GetStyles(), m_cListCtrl.GetExtendedStyle());
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnGetSelected

	Handler for the "Get Selected" button.  We step through the list 
	control using the GetFirstSelectedItemPosition call.  As we loop through
	and find slected items, we create a string which is eventually dumped
	to the log edit control.
	  
	This could also be done using a loop such as 
	
	int n = GetNextItem(-1, LVNI_SELECTED);
	while (n >= 0)
	{
		// Do somthing with item n
	
		n = GetNextItem(n, LVNI_SELECTED);
	}
		
		  	
	Params
		None

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnGetSelected() 
{
	// Get the selected items in the control
	POSITION p = m_cListCtrl.GetFirstSelectedItemPosition();
	CString msg = _T("");
	while (p)
	{
		int nSelected = m_cListCtrl.GetNextSelectedItem(p);
		if (msg.IsEmpty())
			msg = _T("The following items are selected: ");

		CString str;
		str.Format(_T("%s"), m_cListCtrl.GetItemText(nSelected, 0));

		if (p)
			str += _T(", ");
		msg += str;

	}
	if (msg.IsEmpty())
		msg = _T("There are no selected items");

	m_Log.AppendString(msg);




}

/////////////////////////////////////////////////////////////////////////////
/*
	RecreateListControl

	Some CListCtrl styles cannot be dynamically set once the window has been
	created.  For instance, LVS_NOLABELWRAP, LVS_NOCOLUMNHEADER, and 
	LVS_NOSORTHEADER all require control recreation.  Rather than check for 
	individual style flags, we just recreate the control whenever the user
	changes the list control states.
  
	Params
		dwStyles	ListControl styles to use in CListCtrl::Create()
		dwStylesEx	Extended styles to apply to created CListCtrl

	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::RecreateListControl(const DWORD &dwStyles, const DWORD &dwStylesEx)
{

	CRect rect;
	m_cListCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_cListCtrl.DestroyWindow();
	m_cListCtrl.Create(dwStyles, rect, this, IDC_LIST_CTRL);

	// CListCtrl::SetExtendedStyle works differently for LVS_EX_XXX styles than
	// CWnd::ModifyStyleEx works for CWnd extended styles.  It sends a LVM_SETEXTENDEDLISTVIEWSTYLE
	// message to the underlying CListCtrl, whereas ModifyStyleEx resolves to _AfxModifyStyle, 
	// which turns into ::SetWindowLong(GWL_EXSTYLE).  These calls don't seem to give the
	// same results.  For extended styles, always use CListCtrl::SetExtendedStyle or 
	// ListView_SetExtendedListViewStyle
	m_cListCtrl.SetExtendedStyle( dwStylesEx);

	// For standard CWnd extended styles, you can use this:
	m_cListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// Re attach the image lists
	m_cListCtrl.SetImageList(&m_cImageListNormal, LVSIL_NORMAL);
	m_cListCtrl.SetImageList(&m_cImageListSmall, LVSIL_SMALL);

	// Re create the columns for report mode
	InitListCtrlCols();

	// Insert the items again
	InsertItems(); // repopulate with a new item group

	// Show the image if needed
	EnableBkImage(m_bImage);
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnItemSelect

	When the user enters an index that they would like to select, first
	check the the index is in bounds.  If so, set the state of the item
	and ensure that the item is visible in the list.
  
	Params
		None
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnItemSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// Select this item in the list control
	int nCount = m_cListCtrl.GetItemCount();
	if (m_nSelectItem >= nCount)
	{
		AfxMessageBox(_T("You entered an out of range index."));
		m_nSelectItem = 0;
	}
	else
	{
		m_cListCtrl.SetItemState(m_nSelectItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		m_cListCtrl.EnsureVisible(m_nSelectItem, FALSE);
	}
	UpdateData(FALSE);

}


/////////////////////////////////////////////////////////////////////////////
/*
	OnItemchangedListCtrl

	Handler for the LVN_ITEMCHANGED message which is sent in response
	to the user changing some item in the list control
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CString str;
	str.Format(_T("OnItemchangedListCtrl.  Item index: %d"), pNMListView->iItem);
	m_Log.AppendString(str);


	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnItemchangedListCtrl

	Handler for the NM_CLICK message which is sent in response
	to the user changing left-clicking somewhere in the control
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	CPoint point;
	::GetCursorPos(&point);
	CString str;
	str.Format(_T("OnClickListCtrl.  Cursor position is x: %d, y: %d"), point.x, point.y);
	m_Log.AppendString(str);
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnItemchangedListCtrl

	Handler for the NM_RCLICK message which is sent in response
	to the user right clicking somewhere in the control.  This might 
	be a good place to show a context menu.
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnRclickListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Log.AppendString(_T("CListCtrlDemoDlg::OnRclickListCtrl"));
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	CString str;
	str.Format(_T("\tMouse position: x=%d, y=%d"), ptCursor.x, ptCursor.y);
	m_Log.AppendString(str);




	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnItemchangedListCtrl

	Handler for the NM_RCLICK message which is sent in response
	to the user right clicking somewhere in the control.  This might 
	be a good place to show a context menu.
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnClearLog() 
{
	// TODO: Add your control notification handler code here
	m_Log.SetWindowText(_T(""));
}



/////////////////////////////////////////////////////////////////////////////
/*
	OnInitDialog

	Setup the email adress
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cEmail.SetURL(_T("mailto:matt_weagle@hotmail.com"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnBkImage

	Toggle the state of the background image
  
	Params
		<Standard>
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::OnBkImage() 
{
	// TODO: Add your control notification handler code here
	m_bImage = !m_bImage;
	EnableBkImage(m_bImage);
}


/////////////////////////////////////////////////////////////////////////////
/*
	EnableBkImage

	Helper function to show/hide the background image.  It's factored
	out of the EnableBkImage call because the control needs to be 
	recreated when the user changes the Extended Styles
  
	Params
		BOOL		show/hide the image
	
	Returns 
		void
*/
/////////////////////////////////////////////////////////////////////////////
void CListCtrlDemoDlg::EnableBkImage(BOOL bEnable)
{


	if (bEnable)
	{
		/* 
		There are two ways to include a background image:
			(1) Specify a valid filepath and load the bitmap handle from there
			(2) Specify a valid URL using the res:protocol (preferred).

		(2) Is preferable because you don't need to include a separate file 
		with your application.  You can embed the BMP in the resource fork
		and use the RES: protocol to have the ListControl load it.  See
		http://www.microsoft.com/mind/0199/cutting/cutting0199.asp for more
		information about the RES: syntax.  This idea was suggested by Nick Hodapp
		(http://www.codetools.com/script/Articles/list_articles.asp?userid=162)

		Both methods are included for completeness, but the sample
		application uses the embedded BMP.  Note that specifying a HBITMAP
		in the struct is not currently supported.
		*/
		  
		// Load the resource and apply it to the background
		TCHAR szBuffer[_MAX_PATH]; 
		VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		CString sPath;
		sPath.Format(_T("res://%s/#2/#142"),szBuffer);

		LVBKIMAGE bki;
		bki.ulFlags = LVBKIF_STYLE_TILE | LVBKIF_SOURCE_URL ;
		bki.pszImage = sPath.GetBuffer(sPath.GetLength());
		bki.cchImageMax = sPath.GetLength();
		VERIFY(m_cListCtrl.SetBkImage( &bki));




		/*
		Construct the path to the BMP.  Although the CListCtrl::SetBkImage docs
		indicate that you can use an HBITMAP, the latest docs on the LVBKIMAGE
		underlying struct indicates that the LVBKIMAGE.hbm member is not used.  
		*/
		/*
		TCHAR szBuffer[_MAX_PATH]; 
		VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		CString sPath = (CString)szBuffer;
		sPath = sPath.Left(sPath.ReverseFind('\\') + 1);	
		sPath += "bk.bmp";
		m_cListCtrl.SetBkImage( sPath.GetBuffer(sPath.GetLength()), TRUE);
		sPath.ReleaseBuffer();
		*/

		
		// Whichever one you choose, log the path used
		CString str;
		str.Format(_T("Set background image: %s"), sPath.GetBuffer(sPath.GetLength()));
		m_Log.AppendString(str);
		sPath.ReleaseBuffer();

	}
	else
	{
		m_cListCtrl.SetBkImage( HBITMAP(0));
		m_Log.AppendString(_T("Cleared background image."));

	}
}
