/*****************************************************************************
*@File:IceDll.cpp
*@Author:Ice
*@Date:Mar,2008
*/
#include "IceDll.h"
//////////////////////////////////////////////////////////////////////////////
HMODULE IcyLoadLibrary(LPCTSTR lpFileName,  DWORD dwFlags)
{
	HMODULE hModule = GetModuleHandle(lpFileName);
	if (NULL == hModule)
	{
		return LoadLibraryEx(lpFileName, 0 , dwFlags);
	}
	else
	{
		return hModule;
	}
}
//////////////////////////////////////////////////////////////////////////////
BOOL IcyIsModuleLoaded(LPCTSTR lpFileName)
{
	if (NULL == GetModuleHandle(lpFileName))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
//////////////////////////////////////////////////////////////////////////////
BOOL IcyFreeLibrary(HMODULE hMoudule)
{
	return FreeLibrary(hMoudule);
}

VOID IcyFreeLibraryAndExitThread(
  HMODULE hModule,  // handle to the DLL module
  DWORD dwExitCode  // exit code for thread
)
{
	FreeLibraryAndExitThread(hModule, dwExitCode);
}
//////////////////////////////////////////////////////////////////////////////
DWORD IcyGetModuleFileName(
  HMODULE hModule,    // handle to module
  LPTSTR lpFilename,  // path buffer
  DWORD nSize        // maximum characters to retrieve
)
{
	return GetModuleFileName(hModule, lpFilename, nSize);
}
//////////////////////////////////////////////////////////////////////////////
HANDLE IcyGetModuleHandle(LPTSTR lpFilename)
{
	return GetModuleHandle(lpFilename);
}
//////////////////////////////////////////////////////////////////////////////
FARPROC IcyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return GetProcAddress(hModule, lpProcName);
}
//////////////////////////////////////////////////////////////////////////////
