// ChangeNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Example1.h"
#include "ChangeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeNameDlg dialog


//##ModelId=46F43D0C039B
CChangeNameDlg::CChangeNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeNameDlg)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}


//##ModelId=46F43D0C03AB
void CChangeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeNameDlg)
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeNameDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeNameDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeNameDlg message handlers
