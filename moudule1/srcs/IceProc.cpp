#include "IceProc.H"
/******************************************************************************\
*									引入函数
/******************************************************************************/
extern HERROR IcyEnumWindows(IN WND_VECTOR &wndVctor, IN HWND hwdParent = NULL);
extern INT IcyGetWindowText(HWND hWnd, PSTR strBuf, DWORD dwMaxRcv = 255 );
/******************************************************************************\
*									引入变量
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
VOID ShowProcessInfo(DWORD dwProcessID) {

   //SetWindowText(hwnd, TEXT(""));   // Clear the output box

   CToolhelp th(TH32CS_SNAPALL, dwProcessID);

   // Show Process details
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = th.ProcessFirst(&pe);
   for (; fOk; fOk = th.ProcessNext(&pe)) {
      if (pe.th32ProcessID == dwProcessID) {
         printf(TEXT("Filename: %s\r\n"), pe.szExeFile);
         printf(TEXT("   PID=%08X, ParentPID=%08X, ")
            TEXT("PriorityClass=%d, Threads=%d, Heaps=%d\r\n"),
            pe.th32ProcessID, pe.th32ParentProcessID, 
            pe.pcPriClassBase, pe.cntThreads,
            th.HowManyHeaps());
         break;   // No need to continue looping
      }
   }

   // Show Modules in the Process
   // Number of characters to display an address
   const int cchAddress = sizeof(PVOID) * 2;
   printf(TEXT("\r\nModules Information:\r\n")
      TEXT("  Usage  %-*s(%-*s)  %8s  Module\r\n"),
      cchAddress, TEXT("BaseAddr"),
      cchAddress, TEXT("ImagAddr"), TEXT("Size"));

   MODULEENTRY32 me = { sizeof(me) };
   fOk = th.ModuleFirst(&me);
   for (; fOk; fOk = th.ModuleNext(&me)) {
      if (me.ProccntUsage == 65535) {
         // Module was implicitly loaded and cannot be unloaded
         printf(TEXT("  Fixed"));
      } else {
         printf(TEXT("  %5d"), me.ProccntUsage);
      }
      PVOID pvPreferredBaseAddr = 
         GetModulePreferredBaseAddr(pe.th32ProcessID, me.modBaseAddr);
      if (me.modBaseAddr == pvPreferredBaseAddr) {
         printf(TEXT("  %p %*s   %8u  %s\r\n"), 
            me.modBaseAddr, cchAddress, TEXT(""), 
            me.modBaseSize, me.szExePath);
      } else {
         printf(TEXT("  %p(%p)  %8u  %s\r\n"), 
            me.modBaseAddr, pvPreferredBaseAddr, me.modBaseSize, me.szExePath);
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////////
VOID ShowModuleInfo(LPCTSTR pszModulePath) {

   //SetWindowText(hwnd, TEXT(""));   // Clear the output box

   CToolhelp thProcesses(TH32CS_SNAPPROCESS);
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = thProcesses.ProcessFirst(&pe);
   printf(TEXT("Pathname: %s\r\n\r\n"), pszModulePath);
   printf(TEXT("Process Information:\r\n"));
   printf(TEXT("     PID    BaseAddr  Process\r\n"));
   for (; fOk; fOk = thProcesses.ProcessNext(&pe)) {
      CToolhelp thModules(TH32CS_SNAPMODULE, pe.th32ProcessID);
      MODULEENTRY32 me = { sizeof(me) };
      BOOL fOk = thModules.ModuleFirst(&me);
      for (; fOk; fOk = thModules.ModuleNext(&me)) {
        // if (_tcscmp(me.szExePath, pszModulePath) == 0)
		  {
            printf(TEXT("  %08X  %p  %s\r\n"), 
               pe.th32ProcessID, me.modBaseAddr, pe.szExeFile);
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////////
PVOID GetModulePreferredBaseAddr(DWORD dwProcessId, PVOID pvModuleRemote) {

   PVOID pvModulePreferredBaseAddr = NULL;
   IMAGE_DOS_HEADER idh;
   IMAGE_NT_HEADERS inth;

   // Read the remote module's DOS header
   Toolhelp32ReadProcessMemory(dwProcessId, 
      pvModuleRemote, &idh, sizeof(idh), NULL);

   // Verify the DOS image header
   if (idh.e_magic == IMAGE_DOS_SIGNATURE) {
      // Read the remote module's NT header
      Toolhelp32ReadProcessMemory(dwProcessId, 
         (PBYTE) pvModuleRemote + idh.e_lfanew, &inth, sizeof(inth), NULL);

      // Verify the NT image header
      if (inth.Signature == IMAGE_NT_SIGNATURE) {
         // This is valid NT header, get the image's preferred base address
         pvModulePreferredBaseAddr = (PVOID) inth.OptionalHeader.ImageBase;
      }
   }
   return(pvModulePreferredBaseAddr);
}
//////////////////////////////////////////////////////////////////////////////////////
HERROR IcyGetWindowThreadProcessId(HWND hWnd,  DWORD &dwProcessId, DWORD &dwThreadId)
{	
	dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
	DWORD dwErrCode  = GetLastError();
	if (SUCCESS != dwErrCode)
	{
		ICY_ASSERT2(FALSE, dwErrCode);
		return FAILE;
	}
	return SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////
HERROR IcyGetWindowThreadProcessId(LPCSTR pWndText, DWORD &dwProcessId, DWORD &dwThreadId)
{
	DWORD tmpProcessId = 0;
	DWORD tmpThreadId  = 0;
	CHAR strBuf[255] = {0};

	IcyRefreshWindowHandles();

	for (DWORD i = 0; i < g_vWnd.size(); i++)
	{
		INT len = IcyGetWindowText(g_vWnd[i], strBuf);

		if ((len > 0) && IcyStringMatch(pWndText, strBuf))
		{
			return IcyGetWindowThreadProcessId(g_vWnd[i], dwProcessId, dwThreadId);
		} 

	} /* end for */

	return FALSE;	
}
////////////////////////////////////////////////////////////////////////////////
BOOL IcyGetTopWindowOfProc(IN DWORD dwProcessId, OUT HWND &hWnd)
{
	DWORD tmpProcessId = 0;
	DWORD tmpThreadId  = 0;

	IcyRefreshWindowHandles();
	for (DWORD i = 0; i < g_v_wnd_parent_childv.size(); i++)
	{
		IcyGetWindowThreadProcessId(g_v_wnd_parent_childv[i].m_hParent,
			tmpProcessId, tmpThreadId);
		if (dwProcessId == tmpProcessId)
		{
			hWnd = g_v_wnd_parent_childv[i].m_hParent;
			return TRUE;
		}
	} /* end for */
	return FALSE;	
}
////////////////////////////////////////////////////////////////////////////////
BOOL IcyGetTopWindowOfProc(IN HANDLE hProcess, OUT HWND &hWnd)
{
	//OpenProcess()
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
BOOL IcyEnumProcess(IN vector<DWORD> &vProcId)
{
	CToolhelp th(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk = th.ProcessFirst(&pe);
	for (; fOk; fOk = th.ProcessNext(&pe))
	{
		vProcId.push_back(pe.th32ProcessID);     
	}
	return TRUE;	
}
////////////////////////////////////////////////////////////////////////////////

BOOL IcyGetProcEntry(IN DWORD dwProcId, IO PROCESSENTRY32 &pe)
{
	CToolhelp th(TH32CS_SNAPALL, 0);
	BOOL fOk = th.ProcessFirst(&pe);
	for (; fOk; fOk = th.ProcessNext(&pe))
	{
		if (dwProcId == pe.th32ProcessID)
		{
			return TRUE;
		}
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////
HERROR IcyGetParentProcessId(IN DWORD dwSubId, OUT DWORD &dwParentId)
{
	CToolhelp th(TH32CS_SNAPALL, 0);

	PROCESSENTRY32 pe = { sizeof(pe) };

	BOOL fOk = th.ProcessFirst(&pe);
	for (; fOk; fOk = th.ProcessNext(&pe))
	{
		if (dwSubId == pe.th32ProcessID)
		{
			dwParentId = pe.th32ParentProcessID;
			return SUCCESS;
		}
	}
	return FAILE;
}
////////////////////////////////////////////////////////////////////////////////

HERROR EnableDebugPrivilege(IN HANDLE hProcessHandle, IN BOOL bEnable /* = TRUE */)
{
	BOOL fOk = FALSE;    // Assume function fails
	HANDLE hToken;
	// Try to open this process's access token
	if (OpenProcessToken(hProcessHandle, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		 // Attempt to modify the "Debug" privilege
		 TOKEN_PRIVILEGES tp;
		 tp.PrivilegeCount = 1;
		 LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		 tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		 AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		 fOk = (GetLastError() == ERROR_SUCCESS);
		 CloseHandle(hToken);
	}

	return SUCCESS;
}
