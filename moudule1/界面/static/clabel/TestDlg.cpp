// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testbed.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		
	
	//}}AFX_DATA_MAP
	int x0 = IDC_X0_D0;
		int x1 = IDC_X1_D0;
		int x2 = IDC_X2_D0;
		int x3 = IDC_X3_D0; 
		int i = 0;
		while (i <= 8)
		{
			DDX_Control(pDX, x0+i, m_ctrlX0[i]);
			DDX_Control(pDX, x1+i, m_ctrlX1[i]);
			DDX_Control(pDX, x2+i, m_ctrlX2[i]);
			DDX_Control(pDX, x3+i, m_ctrlX3[i]);
			i++;
		}
	
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	for(int i = 0; i < 9; i++)
	{
		m_ctrlX0[i].SetBkColor(RGB(255,0,0));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
