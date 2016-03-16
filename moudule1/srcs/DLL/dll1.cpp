#include "dll1.h"
LRESULT CALLBACK HookTest(int code, WPARAM wParam, LPARAM lParam)
{
	printf("I am doing the hook test!\n");
	
	return SUCCESS;
}