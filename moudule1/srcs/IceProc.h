/*****************************************************************************\
*                                                                             *
* IceProc.h -  Functions refered to proccess and threads						  *
*                                                                             *
* Version 1.0                                                                 *
*                                                                             *
* NOTE:																		  *
*                                                                             *
* Copyright 2008 - 2012 Ice.   All rights reserved.							  *
*                                                                             *
\*****************************************************************************/
#ifndef _ICY_PROC_H
#define _ICY_PROC_H
#include "Ice.h"
#include "Toolhelp.h"
/* If the file name ends in a period (.) with no extension, or if the file name contains a path, .exe is not appended. If the file name does not contain a directory path, the system searches for the executable file in the following sequence: 

The directory from which the application loaded. 
The current directory for the parent process. 
Windows 95/98/Me: The Windows system directory. Use the GetSystemDirectory function to get the path of this directory. 
Windows NT/2000/XP: The 32-bit Windows system directory. Use the GetSystemDirectory function to get the path of this directory. The name of this directory is System32. 

Windows NT/2000/XP: The 16-bit Windows system directory. There is no function that obtains the path of this directory, but it is searched. The name of this directory is System. 
The Windows directory. Use the GetWindowsDirectory function to get the path of this directory. 
The directories that are listed in the PATH environment variable. 

lpProcessAttributes 
[in] Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes. If lpProcessAttributes is NULL, the handle cannot be inherited. 
Windows NT/2000/XP: The lpSecurityDescriptor member of the structure specifies a security descriptor for the new process. If lpProcessAttributes is NULL, the process gets a default security descriptor. 

lpThreadAttributes 
[in] Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes. If lpThreadAttributes is NULL, the handle cannot be inherited. 
Windows NT/2000/XP: The lpSecurityDescriptor member of the structure specifies a security descriptor for the main thread. If lpThreadAttributes is NULL, the thread gets a default security descriptor. 

bInheritHandles 
[in] Indicates whether the new process inherits handles from the calling process. If TRUE, each inheritable open handle in the calling process is inherited by the new process. Inherited handles have the same value and access privileges as the original handles. 

The STARTUPINFO structure is used with the CreateProcess function to specify main window properties if a new window is created for the new process. For graphical user interface (GUI) processes, this information affects the first window created by the CreateWindow function and shown by the ShowWindow function. For console processes, this information affects the console window if a new console is created for the process. A process can use the GetStartupInfo function to retrieve the STARTUPINFO structure specified when the process was created. 

typedef struct _STARTUPINFO { 
    DWORD   cb; 
    LPTSTR  lpReserved; 
    LPTSTR  lpDesktop; 
    LPTSTR  lpTitle; 
    DWORD   dwX; 
    DWORD   dwY; 
    DWORD   dwXSize; 
    DWORD   dwYSize; 
    DWORD   dwXCountChars; 
    DWORD   dwYCountChars; 
    DWORD   dwFillAttribute; 
    DWORD   dwFlags; 
    WORD    wShowWindow; 
    WORD    cbReserved2; 
    LPBYTE  lpReserved2; 
    HANDLE  hStdInput; 
    HANDLE  hStdOutput; 
    HANDLE  hStdError; 
} STARTUPINFO, *LPSTARTUPINFO; 
*/


// BOOL CreateProcess(
//   LPCTSTR lpApplicationName,                 // name of executable module
//   LPTSTR lpCommandLine,                      // command line string
//   LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
//   LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
//   BOOL bInheritHandles,                      // handle inheritance option
//   DWORD dwCreationFlags,                     // creation flags
//   LPVOID lpEnvironment,                      // new environment block
//   LPCTSTR lpCurrentDirectory,                // current directory name
//   LPSTARTUPINFO lpStartupInfo,               // startup information
//   LPPROCESS_INFORMATION lpProcessInformation // process information
// );

/*******************************************************************************
要枚举顶层窗口，请使用 EnumWindows() 函数。不要使用 GetWindow() 创建自己的窗口列表
，因为它可能受 z 轴次序更改和丢失窗口的干扰。
*/

////////////////////////////////////////////////////////////////////////////////
VOID ShowProcessInfo(DWORD dwProcessID);

VOID ShowModuleInfo(LPCTSTR pszModulePath);

PVOID GetModulePreferredBaseAddr(DWORD dwProcessId, PVOID pvModuleRemote);
/* 通过进程句柄获得exe文件名  */
// HERROR GetModuleNameByProcHandle(HANDLE hProcess, LPSTR szModuleName);

/* 根据窗口句柄得到其所属线程,进程ID */
HERROR IcyGetWindowThreadProcessId(HWND hWnd,  DWORD &dwProcessId, DWORD &dwThreadId);

HERROR IcyGetWindowThreadProcessId(LPCSTR pWndText, DWORD &dwProcessId, DWORD &dwThreadId);

BOOL IcyGetTopWindowOfProc(IN DWORD dwProcessId, OUT HWND &hWnd);

BOOL IcyGetTopWindowOfProc(IN HANDLE hProcess, OUT HWND &hWnd);

BOOL IcyEnumProcess(IN vector<DWORD> &vProcId);

// 获取进程dwProcId的PE结构信息，不存在返回FALSE */
BOOL IcyGetProcEntry(IN DWORD dwProcId, IO PROCESSENTRY32 &pe);

// 取得进程的父进程ID
HERROR IcyGetParentProcessId(IN DWORD dwSubId, OUT DWORD &dwParentId);


// 提升对进程访问权限为调试权限
HERROR EnableDebugPrivilege(IN HANDLE hProcessHandle, IN BOOL bEnable = TRUE);
////////////////////////////////////////////////////////////////////////////////

#endif
