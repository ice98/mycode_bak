/**********************
*@File:IceDll.h
*@Author:Ice
*@Date:Mar 3rd,2008
***********************/
#ifndef _ICE_DLL_H
#define _ICE_DLL_H

#include "Ice.h"
/* dwFlags entry-point execution option
DONT_RESOLVE_DLL_REFERENCES: System do not call dllMain for dll.
LOAD_LIBRARY_AS_DATAFILE: Usable for resources dll,and exe loading.
LOAD_WITH_ALTERED_SEARCH_PATH: Chage the dll/exe path searching order.*/ 
HMODULE IcyLoadLibrary(LPCTSTR lpFileName,  DWORD dwFlags);
BOOL IcyFreeLibrary(HMODULE hMoudule);
VOID IcyFreeLibraryAndExitThread(HMODULE hModule, DWORD dwExitCode);
BOOL IcyIsModuleLoaded(LPCTSTR lpFileName);
DWORD IcyGetModuleFileName(
  HMODULE hModule,    // handle to module
  OUT LPTSTR lpFilename,  // path buffer
  DWORD nSize         // maximum characters to retrieve
  //HANDLE hProcess = NULL    // handle to process
);
HANDLE IcyGetModuleHandle(LPTSTR lpFilename = NULL);
FARPROC IcyGetProcAddress(HMODULE hModule, LPCSTR lpProcName);


#endif
