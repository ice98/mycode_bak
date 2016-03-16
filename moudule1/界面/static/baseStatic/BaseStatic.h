#if !defined(AFX_BASESTATIC_H__4F9902E6_6D90_4A42_8C2F_8C09CB01DDC3__INCLUDED_)
#define AFX_BASESTATIC_H__4F9902E6_6D90_4A42_8C2F_8C09CB01DDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseStatic window

class CBaseStatic : public CStatic
{
// Construction
public:
	CBaseStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextSize(int nSize);
	void SetTextColor(COLORREF nColor);
	COLORREF m_bkColor;
	COLORREF m_TextColor;
	void SetBkColor(COLORREF nColor);
	virtual ~CBaseStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_nSize;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASESTATIC_H__4F9902E6_6D90_4A42_8C2F_8C09CB01DDC3__INCLUDED_)
