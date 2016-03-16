#ifndef __ICE_COLOR_H__
#define __ICE_COLOR_H__

#include "IceBase.h"

COLORREF IcyGetColor(HWND hWnd, INT x, INT y);
BOOL IcyFindColor(HWND hWnd, COLORREF color, INT l, INT t, INT r, INT b, INT &x, INT &y);

CDC *IcyGetDCAndRect(HWND hWnd, CRect &rect, BOOL bClient = TRUE);

#include "DIBAPI.H"

class CIcyBitmap
{
public:

    CIcyBitmap();
    virtual ~CIcyBitmap();

    BOOL Open(LPCTSTR lpszPathName);
    BOOL Open(HWND hWnd = NULL, BOOL bClient = FALSE);


    BOOL Save(LPCTSTR lpszPathName);
    BOOL Save();

    DWORD GetInfo(DWORD &dwWidth, DWORD &dwHeight, BYTE &byBitCount);

    COLORREF Color(INT x, INT y);
    BOOL FindColor(COLORREF color, 
                            INT left1, INT top1, 
                            INT left2, INT top2, 
                            INT &x, INT &y);
        
    const CString& GetPathName() const
    {
        return m_strPathName;
    }

    static BOOL ScreenSave(LPCTSTR pszFileName, HWND hWnd = NULL, BOOL bClient = FALSE);
    static BOOL ScreenDisplay(LPCTSTR pszFileName, HWND hWnd = NULL, BOOL bClient = FALSE);  

    static BOOL FindBmp(int x1,int y1,int x2,int y2,LPCTSTR lpszName,int &fx,int &fy);

    static BOOL FindBmpEx(int x1,int y1,int x2,int y2,LPCTSTR lpszName,
                          int backcolor,int errorcolor,int errorcount,int &fx,int &fy);

protected:

    static BOOL Save2BMP(LPCTSTR pszFileName, CDC &memDC, CBitmap &memBitmap, CRect &rect);
    static BOOL Save2FILE(LPCTSTR pszFileName, CBitmap &memBitmap);
    
protected:
    HDIB m_hDIB;

    CString m_strPathName;
    int m_nColorIndex;
    BOOL m_bOpen;

    DWORD m_dwWidth;
    DWORD m_dwHeight;
    BYTE m_byColorBits;

    LPBITMAPINFOHEADER    m_lpDIBHdr;            // BITMAPINFOHEADER指针,实际可以使用LPBITMAPINFOHEADER
	LPSTR    m_lpDIBBits;           // DIB象素指针
private:

    BOOL InitInfo();
};

#endif

