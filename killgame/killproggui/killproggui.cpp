// killproggui.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "killproggui.h"
#include "killprogguiDlg.h"
//////////////////////////////////////////////////////////////////////////

#include <tlhelp32.h>
#include <iostream>
using namespace std;

#define PROG_FILE_NAME "prog.txt"
#define MAX_KILL 20
char g_acExe2Kill[MAX_KILL][30] = {0};
int g_ItemNums = 0;
FILE *fp;

BOOL ReadKillProc();
BOOL FindAndKillProcess();
BOOL Search(char *strPeName);
BOOL KillProcess(DWORD dwPID);
DWORD WINAPI ThreadProc1(LPVOID lParam);
char *str2lower(char *zstr);
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKillprogguiApp

BEGIN_MESSAGE_MAP(CKillprogguiApp, CWinApp)
	//{{AFX_MSG_MAP(CKillprogguiApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillprogguiApp construction

CKillprogguiApp::CKillprogguiApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKillprogguiApp object

CKillprogguiApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKillprogguiApp initialization

BOOL CKillprogguiApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	HANDLE h_Thread1;
	h_Thread1 = ::CreateThread(NULL,
					0,
					ThreadProc1,
					NULL,
					0,
					NULL);

		if ( NULL == h_Thread1)
		{
			cout << "Can't create process snapshot!" << endl;
		}


		while (1)
		{
			Sleep(5000);
		}

// 	CKillprogguiDlg dlg;
// 	m_pMainWnd = &dlg;	
// 
// 
	
// 	int nResponse = dlg.DoModal();
// 
// 	if (nResponse == IDOK)
// 	{
// 	}
// 	else if (nResponse == IDCANCEL)
// 	{
// 	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc1(LPVOID lParam)
{

	while(1)
	{
		if(!ReadKillProc())
		{
			return 1;
		}
		FindAndKillProcess();
		Sleep(5000);
	}
}

BOOL ReadKillProc()
{
	fp = fopen(PROG_FILE_NAME, "r+");
	if (NULL == fp)
	{
		AfxMessageBox("File open failed!");
		return FALSE;
	}

	int i = 0;
	fflush(fp);
	fseek(fp, 0, SEEK_SET);
	while ((!feof(fp)) && (i < MAX_KILL))
	{
		memset(g_acExe2Kill[i], 0, sizeof(g_acExe2Kill[i]));
		fgets(g_acExe2Kill[i], sizeof(g_acExe2Kill[i]), fp);

		// 去掉fgets的换行符
		char *p = g_acExe2Kill[i];
		while ((*p != 0x0A) && (*p != 0))
			p++;
		*p = 0;
	    // end of 去掉fgets的换行符

		str2lower(g_acExe2Kill[i]);
		i++;
	//	fscanf(fp, g_acExe2Kill[i]);
	}
	g_ItemNums = i;
	fclose(fp);
	return TRUE;
}

BOOL FindAndKillProcess()
{
	HANDLE hProcessSnap  = NULL;
	PROCESSENTRY32 pe32  = {0};

	pe32.dwSize = sizeof(PROCESSENTRY32);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hProcessSnap == INVALID_HANDLE_VALUE )
	{
		AfxMessageBox("Can't create process snapshot!");
		return FALSE;
	}

	if (Process32First(hProcessSnap, &pe32) == FALSE)
	{
		AfxMessageBox("Process32First error!");
		return FALSE;
	}
	
	do 
	{	
		if (Search(pe32.szExeFile))
		{
			if (!KillProcess(pe32.th32ProcessID))
			{
				CString str;
				str.Format("Kill %s failed!", pe32.szExeFile);
				AfxMessageBox(str);
			}
		}
	
	} while(Process32Next(hProcessSnap,&pe32));

	CloseHandle(hProcessSnap);
	return TRUE;
}

BOOL KillProcess(DWORD dwPID)
{	
	HANDLE hProcess = NULL;

	hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (!TerminateProcess(hProcess,1))
	{
		return FALSE;		
	}

	return TRUE;
}

// 在配置表中查找，看该进程是否在配置表中存在
BOOL Search(char *strPeName)
{
	str2lower(strPeName);
	for (int i = 0; i < g_ItemNums; i++)
	{
		if (0 == strcmp(g_acExe2Kill[i], strPeName))
		{
			return TRUE;
		}
	}
	return FALSE;	
}

char* str2lower(char * const zstr)
{
	char *p = zstr;
	while (*p != 0)
	{
		if (*p >= 'A' && *p <= 'Z')
		{
			*p = *p - 'A' + 'a';
		}
		p++;
	}
	return zstr;
}