// KeyBoardAppDlg.h : header file
//

#if !defined(AFX_KEYBOARDAPPDLG_H__FCD59F26_695B_4E15_88F7_A946035B6906__INCLUDED_)
#define AFX_KEYBOARDAPPDLG_H__FCD59F26_695B_4E15_88F7_A946035B6906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardAppDlg dialog

class CKeyBoardAppDlg : public CDialog
{
// Construction
public:
	CKeyBoardAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyBoardAppDlg)
	enum { IDD = IDD_KEYBOARDAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDAPPDLG_H__FCD59F26_695B_4E15_88F7_A946035B6906__INCLUDED_)
