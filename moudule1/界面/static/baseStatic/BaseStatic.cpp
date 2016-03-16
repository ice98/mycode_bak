// BaseStatic.cpp : implementation file
//

#include "stdafx.h"
#include "qpglxt.h"
#include "BaseStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseStatic

CBaseStatic::CBaseStatic()
{
	m_bkColor=RGB(255,255,255);
	m_TextColor=RGB(0,0,0);
	m_nSize=9;
}

CBaseStatic::~CBaseStatic()
{
}


BEGIN_MESSAGE_MAP(CBaseStatic, CStatic)
	//{{AFX_MSG_MAP(CBaseStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseStatic message handlers

void CBaseStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CFont font,* pfont;
	font.CreatePointFont(m_nSize*10,"ËÎÌו",&dc);
	pfont=dc.SelectObject(&font);
	dc.SetBkColor(m_bkColor);
	dc.SetTextColor(m_TextColor);
	CString sText;
	this->GetWindowText(sText);
	CRect rect;
	this->GetClientRect(&rect);
	dc.DrawText(sText,&rect,DT_LEFT|DT_WORDBREAK);
	::DeleteObject(&font);
	::DeleteObject(pfont);
	// Do not call CStatic::OnPaint() for painting messages
}

void CBaseStatic::SetBkColor(COLORREF nColor)
{
	m_bkColor=nColor;	
}

void CBaseStatic::SetTextColor(COLORREF nColor)
{
	m_TextColor=nColor;
}



void CBaseStatic::SetTextSize(int nSize)
{
	m_nSize=nSize;
}
