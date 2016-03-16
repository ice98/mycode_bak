


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _HOOK_KEY_BOARD_H
#define  _HOOK_KEY_BOARD_H


#ifdef DLL_PROJECT
#define DLL_API  __declspec(dllexport)
#else
#define DLL_API  __declspec(dllimport)
#endif


DLL_API BOOL InstallHook(const char* lpFilePathName);
DLL_API BOOL UnHook();

#endif