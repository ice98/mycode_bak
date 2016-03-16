// FullScreenView.cpp : implementation of the CFullScreenView class
//

#include "stdafx.h"
#include "FullScreen.h"

#include "FullScreenDoc.h"
#include "FullScreenView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFullScreenView

IMPLEMENT_DYNCREATE(CFullScreenView, CScrollView)

BEGIN_MESSAGE_MAP(CFullScreenView, CScrollView)
	//{{AFX_MSG_MAP(CFullScreenView)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CHANGE_BACKGROUND_COLOR, OnChangeBackgroundColor)
	ON_COMMAND(ID_FULL_SCREEN_MODE, OnFullScreenMode)
	ON_UPDATE_COMMAND_UI(ID_FULL_SCREEN_MODE, OnUpdateFullScreenMode)
	ON_COMMAND(ID_CHANGE_TEXT_COLOR, OnChangeTextColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFullScreenView construction/destruction

CFullScreenView::CFullScreenView()
{
	// TODO: add construction code here
	isFull = false;
	background = RGB(255,255,150);
	text = RGB(0,0,0);



	ContextMenu= new CMenu();
	if(!ContextMenu->LoadMenu(IDR_MENU_VIEW)){
		AfxMessageBox("Fail to create context menu");
	}


}

CFullScreenView::~CFullScreenView()
{
}

BOOL CFullScreenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFullScreenView drawing

void CFullScreenView::OnDraw(CDC* pDC)
{
	CFullScreenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(background);
	pDC->SetTextColor(text);


	CFont font;
	font.CreateFont(36,0,0,0,0,1,0,0,1,0,0,0,0,_T("Arial"));
	CFont *pOldFont=pDC->SelectObject(&font);

	
	CString str;
	if(isFull)
		str.Format("CScrollView -full screen mode");
	else
		str.Format("CScrollView -normal mode");
	
	
	
	pDC->DrawText(str,CRect(100,100,600,300),DT_CENTER);

	pDC->SelectObject(pOldFont);


}

void CFullScreenView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 600;
	sizeTotal.cy =1600;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CFullScreenView diagnostics

#ifdef _DEBUG
void CFullScreenView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFullScreenView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFullScreenDoc* CFullScreenView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFullScreenDoc)));
	return (CFullScreenDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFullScreenView message handlers

BOOL CFullScreenView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	{
	//	CWnd::OnEraseBkgnd(pDC);
		CBrush br,*oldbr;
		CRect r;
		GetClientRect(&r);
		br.CreateSolidBrush(background);
		oldbr=pDC->SelectObject(&br);
		pDC->PatBlt(0,0,r.Width(),r.Height(),PATCOPY);

		pDC->SetBkColor(background);

		pDC->SelectObject(&oldbr);
		return TRUE;
	}	



	return CScrollView::OnEraseBkgnd(pDC);
}

void CFullScreenView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	if(ContextMenu->GetSubMenu(0)){
		CMenu *pSubMenu= ContextMenu->GetSubMenu(0);
		
		if(isFull){
			pSubMenu->CheckMenuItem(ID_FULL_SCREEN_MODE,MF_CHECKED );
			pSubMenu->EnableMenuItem(ID_CHANGE_TEXT_COLOR,MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_CHANGE_BACKGROUND_COLOR,MF_GRAYED);
		}
		else{
			pSubMenu->CheckMenuItem(ID_FULL_SCREEN_MODE,MF_UNCHECKED );
			pSubMenu->EnableMenuItem(ID_CHANGE_TEXT_COLOR,MF_ENABLED);
			pSubMenu->EnableMenuItem(ID_CHANGE_BACKGROUND_COLOR,MF_ENABLED);
		}
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			point.x,point.y,this);
	}


	CScrollView::OnRButtonDown(nFlags, point);
}

void CFullScreenView::OnChangeBackgroundColor() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg(background);//GetSysColor(COLOR_WINDOW));
	if (dlg.DoModal()==IDOK)
	{
		background=dlg.GetColor();
	}
	InvalidateRect(NULL);	
}

void CFullScreenView::OnChangeTextColor() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg(text);
	if (dlg.DoModal()==IDOK)
	{
		text=dlg.GetColor();
	}
	InvalidateRect(NULL);	
}


void CFullScreenView::OnFullScreenMode() 
{
	// TODO: Add your command handler code here
	if(!isFull){
		//Zaznaczam, ¿e przechodze do trybu pe³noekranowego
		isFull=true;
		// zapamietuje adres okna, w ktorym byl umieszczony widok (pewnie ramka)
		saveParent=this->GetParent();
		// przypisuje widokowi nowego rodzica - czyli desktop
		this->SetParent(GetDesktopWindow());
		CRect rect;// chodzi o wymiary desktop'u-pulpitu
		GetDesktopWindow()->GetWindowRect(&rect);
		// Ustawiam okno na pulpicie
		this->SetWindowPos(&wndTopMost,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);
	}else
	{
		// wylaczenie trybu pelnoekranowego
		isFull=false;
		// przypisuje widokowi starego rodzica
		this->SetParent( saveParent);
		// pobieram wskaznik do ramki
		// aby ja uaktualnic - zeby okno ktore traci status pelnoekranowy
		// zostalo w niej w odpowiednim miejscu ustawione
		// Robie tak dlatego, ¿e nie wiem jak zapamietac ustawienia
		// okna w ramce - a ulegaja one zmianie gdy wlaczam pelny ekran
		// Sposob prosty - Wyswietlenie jakiegokolwiek ControlBaru
		// bez zmiany jego ustawien - to wiaze sie z wstawienie mojego okna
		// tak jak bylo poprzednio !! ??

		//trza include CMainFrame zrobic
//		CMainFrame *frm=(CMainFrame *)AfxGetMainWnd();
		//frm->ShowControlBars();
//		frm->RecalcLayout();
		//frm->UpdateWindow();
		((CMainFrame *)AfxGetMainWnd())->RecalcLayout();
		

		//frm->ShowControlBar(&frm->m_wndToolBar, frm->m_wndToolBar.IsVisible(), FALSE);
	}

	//isFull =!isFull;
}

void CFullScreenView::OnUpdateFullScreenMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(isFull);
}

