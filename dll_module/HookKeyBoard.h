#ifndef _HOOK_KEY_BOARD_H
#define  _HOOK_KEY_BOARD_H

#ifdef DLL_PROJECT
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

DLL_API BOOL InstallHook(const char* lpFilePathName);
DLL_API BOOL UnHook();
#endif