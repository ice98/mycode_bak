#include <stdio.h>
#include "icefile.h"
#include "iceproc.h"
#include "icedll.h"
#include "IceWnd.h"

#include "IceNode.h"

#include "IceColor.h"





void test(char a[])
{
	printf("size of a is %d", sizeof(a));
}

int main()
{
	char strPath[100] = {0}; 
	char strName[100] = {0};

	HANDLE hFile = 0;
	HANDLE hMap = 0;
	PBYTE pAddr = NULL;

// 	DWORD dwProId = GetCurrentProcessId();
//	DWORD dwThreadId = GetCurrentThreadId();
// 	ShowProcessInfo(dwProId);
	//ShowModuleInfo("C:\\WINDOWS\\system32\\services.exe");

// 	// ICY_ASSERT0(FALSE);
// 
// 	sprintf(strName, "%s\\myexplorer.exe", strPath);
// 
// 	// HERROR hRet = IcyMapFile(strName, &hFile, &hMap, (LPVOID*)&pAddr, FILE_MAP_WRITE);
// 	HERROR hRet = IcyMapFile(strName, &hFile, &hMap, (LPVOID*)&pAddr, FILE_MAP_WRITE);
// 	ICY_ASSERT0(SUCCESS==hRet);
// 	// printf("%s\n%s\n", strPath, strName);
// 	printf("File name:%s, Addr: 0x%x\n, hFile: 0x%x", strName, pAddr, hFile);
// 
// 	memset(pAddr, 0x39, 1000);
// 	IcyFlushViewOfFile(pAddr, 1000);
// 	IcyCloseHandle(hFile, hMap);
// 	IcyUnmapViewOfFile(pAddr);

// 	HMODULE hModule = IcyLoadLibrary("DLL.dll", DONT_RESOLVE_DLL_REFERENCES);
// 	ICY_ASSERT0(hModule != NULL);
// 	HHOOK hook = IcySetWindowsHookEx(WH_MOUSE, HookTest, hModule, dwThreadId);
// 	ICY_ASSERT0(hook != NULL);
// 	printf("0x%x\n",hModule);

	//HWND hWnd = IcyFindWindow();

//IcySetOtherWindowText((HWND)0x160962, "123456");
/*	IcyCloseWindowByWinName(_TEXT("Example2_ChatRoom"));*/
//	IcyLeftClickBtn((HWND)0x10046a);
	//printf("0x%x\n",hWnd);
//	IcyCloseWindowByClsName("IEFrame");
 	HWND hWnd = 0;
// 	BOOL bret = IcyGetTopWindow(0xf34, hWnd);
// 
// 	bret ? printf("finded\n") : printf("not finded\n");
// 
// 	IcyCloseWindow((HWND)0x90880);
//	IcyLeftClickBtn((HWND)0x60904);

	//BOOL ret = StringMatch(_TEXT("中国"), _TEXT("中国\\人"));
	//HWND hwnd = IcyFindWindow(_TEXT("E:\\moudule\\test"));
// 	IcyGetTopWindowOfProc((DWORD)0x24c, hWnd);
// 	DWORD count = CIcyProcWindow::GetTopWindowCount();
// 	HWND *pWnd = new HWND[count];
// 	DWORD dwCount = 0;
// 	CIcyProcWindow::GetTopWndHanles(pWnd, count, dwCount);
// 	for (DWORD i = 0; i < dwCount; i++)
// 	{
// 		printf("Parent: 0x%x\n", pWnd[i]);
// 		CWndNode* pNode = CIcyProcWindow::GetChildWindow(pWnd[i]);
// 		//for (WND_ITERATOR it = pNode->m_lChildList.begin(); 
// 		//it != pNode->m_lChildList.end(); it++)
// 		for (DWORD j = 0; j < pNode->GetChildCount(); j++)
// 		{
// 			printf("--0x%x\n", pNode->GetChild(j));
// 		}
// 	}
// 
// 	IcyGetTopWindowOfProc((DWORD)0x3dc, hWnd);

	// TestRefreshWindowHandles();
	WND_VECTOR vctor;
	WND_VECTOR vc;
	BOOL bool = IcyGetTopWindowOfProc(0X7A4, hWnd);
	HWND w = IcyFindWindow(_TEXT("关闭"), "Button");
	//IcyCloseWindow();
	//IcyLeftClickBtn(w);
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	IcyGetWindowThreadProcessId(_TEXT("关闭"), dwProcessId, dwThreadId);
// 	IcyGetTopWindowOfProc(dwProcessId, hWnd);
// 	IcyCloseWindow(hWnd);
	//TerminateProcess(dwProcessId, 0);
//	IcyLeftClickBtn((HWND)0xA09FC);
//	IcyLeftDBClickBtn((HWND)0x30040);
// 	ShowProcessInfo(0);
// 	DWORD dwParentId = 0;
// 	HERROR ret = IcyGetParentProcessId(0xD74, dwParentId);


	COLORREF rColor;
	IcyGetColor(NULL,255,168, rColor);

	return 0;

}
