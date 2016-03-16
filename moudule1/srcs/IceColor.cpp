#include "IceColor.h"


HERROR IcyGetColor(HWND hWnd, INT x, INT y, COLORREF &rColor)
{	
	CWnd *pWnd = NULL;

	if (NULL == hWnd)
	{
		pWnd = CWnd::GetDesktopWindow();
	}
	else
	{
		pWnd = CWnd::FromHandle(hWnd);
	}

	CDC *pCDC = pWnd->GetDC();

	rColor = pCDC->GetPixel(x,y);

	return SUCCESS;
}