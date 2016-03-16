// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

using namespace std;
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CTestClientApp oApp;
	oApp.RegisterService(argc, argv);
	return 0;
}
