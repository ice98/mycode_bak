#include <windows.h>
#include <tlhelp32.h>
#include <iostream.h>

VOID KillProcess();
BOOL FindProcessPID();
DWORD WINAPI ThreadProc1(LPVOID lParam);
DWORD PIDS[8] = {0};

DWORD WINAPI ThreadProc1(LPVOID lParam)
{
	while(1)
	{
		Sleep(4000);
		FindProcessPID();
		KillProcess();
	}
}

BOOL FindProcessPID()
{
	HANDLE hProcessSnap  = NULL;
	PROCESSENTRY32 pe32  = {0};

	pe32.dwSize = sizeof(PROCESSENTRY32);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hProcessSnap == INVALID_HANDLE_VALUE )
	{
		cout << "Can't create process snapshot!" << endl;
		return FALSE;
	}

	if (Process32First(hProcessSnap, &pe32) == FALSE)
	{
		cout << "Process32First error!" << endl;
		return FALSE;
	}
	
	do 
	{	
		if (strcmp(pe32.szExeFile,"QQ.EXE") == 0)
		{
			PIDS[0] = pe32.th32ProcessID;
		}

// 		if (strcmp(pe32.szExeFile,"QQGame.exe") == 0)
// 		{
// 			PIDS[0] = pe32.th32ProcessID;
// 		}
// 		
// 		if (strcmp(pe32.szExeFile,"iGame.exe" ) == 0)
// 		{
// 			PIDS[1] = pe32.th32ProcessID;
// 		}
// 		if (strcmp(pe32.szExeFile,"zzllk.exe" ) == 0)
// 		{
// 			PIDS[2] = pe32.th32ProcessID;
// 		}
// 
// 		if (strcmp(pe32.szExeFile,"mdm365.exe" ) == 0)
// 		{
// 			PIDS[3] = pe32.th32ProcessID;
// 		}
	
	} while(Process32Next(hProcessSnap,&pe32));

	CloseHandle(hProcessSnap);
	return TRUE;
}

VOID KillProcess()
{	
	HANDLE hProcess		 = NULL;

	for ( int i = 0; i < sizeof(PIDS)/sizeof(DWORD); i++)
	{
		if (PIDS[i] != 0)
		{
			hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, PIDS[i]);

			if (!TerminateProcess(hProcess,1))
			{
				cout << "TerminateProcess Game error!" << endl;				
			}
			PIDS[i] = 0;
		}	
	}
}

int main()
{
	HANDLE h_Thread1;
	h_Thread1 = CreateThread(NULL,
				0,
				ThreadProc1,
				NULL,
				0,
				NULL);

	if ( NULL == h_Thread1)
	{
		cout << "Can't create process snapshot!" << endl;
	}

	while(1){Sleep(1000);}
	CloseHandle(h_Thread1);

	return 0;

}


