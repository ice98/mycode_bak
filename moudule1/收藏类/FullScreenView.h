// FullScreenView.h : interface of the CFullScreenView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FULLSCREENVIEW_H__C43F578A_08D9_4D9D_9C27_78888CF836B9__INCLUDED_)
#define AFX_FULLSCREENVIEW_H__C43F578A_08D9_4D9D_9C27_78888CF836B9__INCLUDED_

#include "MainFrm.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFullScreenView : public CScrollView
{
protected: // create from serialization only
	CFullScreenView();
	DECLARE_DYNCREATE(CFullScreenView)

// Attributes
public:
	CFullScreenDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFullScreenView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFullScreenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	boolean isFull;
	CWnd *saveParent;
protected:
	COLORREF background;
	COLORREF text;
	CMenu *ContextMenu;

// Generated message map functions
protected:
	//{{AFX_MSG(CFullScreenView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeBackgroundColor();
	afx_msg void OnFullScreenMode();
	afx_msg void OnUpdateFullScreenMode(CCmdUI* pCmdUI);
	afx_msg void OnChangeTextColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FullScreenView.cpp
inline CFullScreenDoc* CFullScreenView::GetDocument()
   { return (CFullScreenDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FULLSCREENVIEW_H__C43F578A_08D9_4D9D_9C27_78888CF836B9__INCLUDED_)
