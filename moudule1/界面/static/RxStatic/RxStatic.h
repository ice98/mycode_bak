#if !defined(AFX_RXSTATIC_H__70E9FD6D_E341_41D9_B347_A09A3AA868C3__INCLUDED_)
#define AFX_RXSTATIC_H__70E9FD6D_E341_41D9_B347_A09A3AA868C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RxStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RxStatic window

class RxStatic : public CStatic
{
// Construction
public:
	RxStatic(COLORREF TextColor=RGB(255,255,255),COLORREF ShadowColor=RGB(0,0,0));
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RxStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF GetShadowColor();
	void SetShadowColor(COLORREF newColor);
	COLORREF GetTextColor();
	void SetTextColor(COLORREF newColor);
	virtual ~RxStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(RxStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CFont m_font;
	COLORREF m_TextColor,m_ShadowColor;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RXSTATIC_H__70E9FD6D_E341_41D9_B347_A09A3AA868C3__INCLUDED_)
