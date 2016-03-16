// RxStatic.cpp : implementation file
//

#include "stdafx.h"
#include "AllTable.h"
#include "RxStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RxStatic

RxStatic::RxStatic(COLORREF TextColor,COLORREF ShadowColor)
{
	m_TextColor=TextColor;
	m_ShadowColor=ShadowColor;
}

RxStatic::~RxStatic()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(RxStatic, CStatic)
	//{{AFX_MSG_MAP(RxStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RxStatic message handlers

void RxStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString sCaption;
	CRect rcClient;
	COLORREF SystemColor=::GetSysColor(COLOR_3DFACE);
	this->GetClientRect(rcClient);
	CBrush brush;
	brush.CreateSolidBrush(SystemColor);
	dc.FillRect(rcClient,&brush);
	brush.DeleteObject();

	this->GetWindowText(sCaption);
	//载入字体
	CFont* pfont;
	pfont=(CFont*)dc.SelectObject(m_font);
	CRect rcText;
	rcText=rcClient;
	rcText.right=rcText.right-2;
	rcText.bottom=rcText.bottom-2;

	dc.SetBkMode(TRANSPARENT);
	//绘制背景
	dc.SetTextColor(m_ShadowColor);
	dc.DrawText(sCaption,rcClient,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//绘制字体
	dc.SetTextColor(m_TextColor);
	dc.DrawText(sCaption,rcText,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	try{
		dc.SelectObject(pfont);
	}
	catch(...)
	{}
	
	// Do not call CStatic::OnPaint() for painting messages
}

void RxStatic::SetTextColor(COLORREF newColor)
{
	m_TextColor=newColor;
}

COLORREF RxStatic::GetTextColor()
{
	return m_TextColor;
}

void RxStatic::SetShadowColor(COLORREF newColor)
{
	m_ShadowColor=newColor;
}

COLORREF RxStatic::GetShadowColor()
{
	return m_ShadowColor;
}

void RxStatic::PreSubclassWindow() 
{
	m_font.CreateFont(38,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"隶书");
	CStatic::PreSubclassWindow();
}
