// module2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "module2.h"
#include "module2Dlg.h"

#include "Icepub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
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
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModule2Dlg dialog

CModule2Dlg::CModule2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModule2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModule2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModule2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModule2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModule2Dlg, CDialog)
	//{{AFX_MSG_MAP(CModule2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModule2Dlg message handlers

BOOL CModule2Dlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CModule2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CModule2Dlg::OnPaint() 
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
HCURSOR CModule2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void copyscreen(HWND hWnd)
{
    //CWnd *pwnd = CWnd::FromHandle(handle);

    //CDC *pDC = pwnd->GetDC();


  

    HDC hDC = GetDC(hWnd);

    CDC *pDC = CDC::FromHandle(hDC);
    
    //创建兼容DC，在里面绘制后COPY到屏幕上//
    CDC MemDC;
    MemDC.CreateCompatibleDC(pDC);
    
    CBitmap MemBitmap;
    //MemBitmap.CreateCompatibleBitmap(pDC, 800, 600);

    MemBitmap.LoadBitmap("E:\\AnJianProject\\pics\\1.bmp");

    BITMAP bmp;

    MemBitmap.GetBitmap(&bmp);

    
    CBitmap *pOldBitmap = MemDC.SelectObject(&MemBitmap);
    
    // 在兼容DC里绘制图像//
    // MydrawDirect(&MemDC); //你的执行函数
    //MemDC.SetBkColor(RGB(255,0,0));

    //将绘制完成的图像COPY到屏幕指定位置//
    pDC->BitBlt(0, 0, 800, 600, &MemDC, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    MemDC.SelectObject(pOldBitmap);
    
    //pwnd->ReleaseDC(pDC);
    ReleaseDC(hWnd, hDC);

     
}

void CModule2Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
// 
    //HWND hwnd = IcyFindWindow("腾讯网新闻");
    
    HWND hwnd = IcyFindWindow("module2-Mic");
// 
// 	COLORREF color = IcyGetColor(hwnd, 63, 91);
// 
    CString say;
// 	say.Format("%#x", color);

    INT x,  y;

    CIcyBitmap objBitmap;
    CIcyBitmap objBitmap2;

    if (!objBitmap.Open("E:\\AnJianProject\\pics\\连连看.bmp"))
            AfxMessageBox("open bitmap fail");

//     if (!objBitmap.Save("E:\\AnJianProject\\pics\\qm_201354_159.bmp"))
//             AfxMessageBox("save bitmap fail");

    COLORREF color = objBitmap.Color(100,100);   
// 
// 
//     if (objBitmap.FindColor(color, 293,140,296,148, x, y))
//     {
//         say.Format("X=%d,y=%d", x,y);
//     }
//     else
//     {
//         say.Format("not find");
//     }

   // copyscreen(hwnd);


/*    AfxMessageBox(say);*/

    


  /*  CIcyBitmap::ScreenSave("E:\\AnJianProject\\pics\\1151.bmp", NULL);*/

    //CIcyBitmap::ScreenDisplay("E:\\AnJianProject\\pics\\1151.bmp",(HWND)1181966, TRUE);

//         if (objBitmap.Open((HWND)921246, TRUE))
//         {
//             COLORREF color1 = objBitmap.Color(248, 181);
//         }
// // 

       
// // 
// //         objBitmap2.Open("E:\\AnJianProject\\pics\\连连看.bmp");
// //         COLORREF color3 = objBitmap2.Color(100, 100);
// 
//         objBitmap.FindColor(color1,51,110,60,130,x, y);

    BOOL bRet = CIcyBitmap::FindBmp(0,0,1024,768,"E:\\AnJianProject\\pics\\baofeng.bmp", x,y);
    BOOL bRet2 = CIcyBitmap::FindBmpEx(0,0,1024,768,
        "E:\\AnJianProject\\pics\\baofeng.bmp",0xB24F05, 0X505050,30, x,y);
          
     return;




 



	
}
