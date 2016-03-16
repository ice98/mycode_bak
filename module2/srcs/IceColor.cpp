#include "IceColor.h"

#include <atlimage.h>

#include "FindProc.h"


COLORREF IcyGetColor(HWND hWnd, INT x, INT y)
{	
    CWnd *pWnd = NULL;
    
    CDC *pCDC = NULL;
    
    if (NULL == hWnd)
    {
        pWnd = CWnd::GetDesktopWindow();
        pCDC = pWnd->GetDC();
    }
    else
    {
        pWnd = CWnd::FromHandle(hWnd);
        pCDC = pWnd->GetWindowDC();
    }

    ICY_ASSET_UNRET0(NULL!= pCDC);

    COLORREF color = pCDC->GetPixel(x,y);

    pWnd->ReleaseDC(pCDC);
    
    return color;
}

BOOL IcyFindColor(HWND hWnd, COLORREF color, INT l, INT t, INT r, INT b, INT &x, INT &y)
{
    CWnd *pWnd = NULL;
    
    CDC *pCDC = NULL;
    
    if (NULL == hWnd)
    {
        pWnd = CWnd::GetDesktopWindow();
        pCDC = pWnd->GetDC();
    }
    else
    {
        pWnd = CWnd::FromHandle(hWnd);
        pCDC = pWnd->GetWindowDC();
    }
    
    ICY_ASSET_UNRET0(NULL!= pCDC);

    for (INT i = l; i <= r; i++)
    {
        for (INT j = t; j <= b; j++)
        {
            if (color == pCDC->GetPixel(i,j))
            {
                x = i;
                y = j;
                return TRUE;
            }
        }
    }

    return FALSE;
}

CDC *IcyGetDCAndRect(HWND hWnd, CRect &rect, BOOL bClient)
{
    if (NULL == hWnd)
    {
        hWnd = GetDesktopWindow();
    }

    HDC hDC = NULL;
    // CWnd *pWnd = CWnd::FromHandle(hWnd);

    if(bClient)
    {
        ::GetClientRect(hWnd, &rect);
        hDC = ::GetDC(hWnd);
        //pDc = pWnd->GetDC();
    }
    else
    {
        ::GetWindowRect(hWnd, &rect);
        hDC = ::GetWindowDC(hWnd);
        //pDc = pWnd->GetWindowDC();
    }

    return CDC::FromHandle(hDC);
}

BOOL CIcyBitmap::ScreenSave(LPCTSTR pszFileName, HWND hWnd, BOOL bClient)
{
    if (NULL == hWnd)
    {
        hWnd = GetDesktopWindow();
    }
 
    CRect rect; 
    CDC *pScrDc = IcyGetDCAndRect(hWnd, rect, bClient);
    ICY_ASSET_RET_LOG(NULL!= pScrDc, "CIcyBitmap::ScreenSave ,pScrDc is NULL", FALSE);
    
    CDC MemDc;
    MemDc.CreateCompatibleDC(pScrDc); 
    
    CBitmap tScrBmp;
    tScrBmp.CreateCompatibleBitmap(pScrDc, rect.Width(), rect.Height()); 

    BITMAP bmpinfo;
    tScrBmp.GetBitmap(&bmpinfo);
    
    CBitmap *ptOldBmp = MemDc.SelectObject(&tScrBmp);
    MemDc.BitBlt(0, 0, rect.Width(), rect.Height(), pScrDc, 0, 0, SRCCOPY);//����  

    Save2FILE(pszFileName, tScrBmp);

    MemDc.SelectObject(ptOldBmp);
    ::ReleaseDC(hWnd, pScrDc->m_hDC);

    return TRUE;
}


BOOL CIcyBitmap::Save2FILE(LPCTSTR pszFileName, CBitmap &memBitmap)
{
    CImage img;   
    img.Attach((HBITMAP)memBitmap);   
    img.Save(pszFileName);   
    img.Detach(); 

    return TRUE;
}

BOOL CIcyBitmap::Save2BMP(LPCTSTR pszFileName, CDC &memDC, CBitmap &memBitmap, CRect &rect)
{
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);                                               // ���λͼ��Ϣ 
    
    FILE *fp = fopen(pszFileName, "w+b");
    
    BITMAPINFOHEADER bih = {0};                                              // λͼ��Ϣͷ
    bih.biBitCount = bmp.bmBitsPixel;                                        // ÿ�������ֽڴ�С
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;                                             // �߶�
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // ͼ�����ݴ�С
    bih.biWidth = bmp.bmWidth;                                               // ���
    
    BITMAPFILEHEADER bfh = {0};                                              // λͼ�ļ�ͷ
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);     // ��λͼ���ݵ�ƫ����
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;            // �ļ��ܵĴ�С
    bfh.bfType = (WORD)0x4d42;
    
    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);                           //д��λͼ�ļ�ͷ
    
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);                           //д��λͼ��Ϣͷ
    
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //�����ڴ汣��λͼ����
    
    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, rect.Height(), p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);                                    //��ȡλͼ����
    
    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);                       //д��λͼ����
    delete [] p;	
    fclose(fp);

    return TRUE;
}

BOOL CIcyBitmap::ScreenDisplay(LPCTSTR pszFileName, HWND hWnd, BOOL bClient)
{

    if (NULL == hWnd)
    {
        hWnd = GetDesktopWindow();
    }
    
    CRect  rect; 
    CDC *pDestDc = IcyGetDCAndRect(hWnd, rect, bClient);

    //���ر���λͼ    
    // bitmap.LoadBitmap(IDB_BITMAP1); //ͼƬID
    HBITMAP hBitmap=(HBITMAP)LoadImage(NULL, pszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    ICY_ASSET_RET_LOG(NULL!= hBitmap, "CIcyBitmap::ScreenDisplay ,hBitmap is NULL", FALSE);

    CBitmap *ptBitmap = CBitmap::FromHandle(hBitmap);
    
    // bitmap.GetBitmap(&bitmapinfo);    

    //�����ڴ�DC
    CDC MemDc;
    MemDc.CreateCompatibleDC(pDestDc);

    //ѡ��λͼ
    CBitmap* pOldBitmap = MemDc.SelectObject(ptBitmap);

    //����λͼ
    pDestDc->BitBlt(0, 0, rect.Width(), rect.Height(),&MemDc, 0, 0, SRCCOPY);

    //�ָ�ԭ��λͼ
    MemDc.SelectObject(pOldBitmap);

    ::ReleaseDC(hWnd, pDestDc->m_hDC);

    return TRUE;
}


CIcyBitmap::CIcyBitmap()
{
    // ��ʼ������
    m_hDIB = NULL;
    m_lpDIBHdr  = NULL;    
    m_lpDIBBits = NULL;

}

CIcyBitmap::~CIcyBitmap()
{
    // �ж�DIB�����Ƿ����
    if (m_hDIB != NULL)
    {
        // ���DIB����
        ::GlobalFree((HGLOBAL) m_hDIB);

        m_hDIB = NULL;
    }

    m_lpDIBHdr  = NULL;

	m_lpDIBBits = NULL;

}


BOOL CIcyBitmap::Open(LPCTSTR lpszPathName) 
{
    CFile file;
    CFileException fe;
    
    // ���ļ�
    if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
    {
        return FALSE;
    }

    m_hDIB = ReadDIBFile(file);

    file.Close();
    
    // �ж϶�ȡ�ļ��Ƿ�ɹ�
    if (m_hDIB == NULL)
    {
        MessageBox(NULL, _T("��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���"),
            "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

        return FALSE;
    }

    // �����ļ�����
    m_strPathName = CString(lpszPathName);
    
    // ����DIB
    m_lpDIBHdr  = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL)m_hDIB);

    // �ҵ�DIBͼ��������ʼλ��
	m_lpDIBBits = FindDIBBits((LPSTR)m_lpDIBHdr);

    if (!InitInfo())
    {
        return FALSE;
    }

    ::GlobalUnlock((HGLOBAL)m_hDIB);
    
    return TRUE;
}

BOOL CIcyBitmap::Open(HWND hWnd, BOOL bClient)
{

    BOOL bRet = FALSE;
#if 1
    LPCTSTR pszFileName = _T("E:\\AnJianProject\\pics\\111tmp.bmp");
    bRet = ScreenSave(pszFileName, hWnd, bClient);

    ICY_ASSET_RET_LOG(bRet, _T("CIcyBitmap::Open ScreenSave fail."), FALSE);
    
    bRet = Open(pszFileName);
    ICY_ASSET_RET_LOG(bRet, _T("CIcyBitmap::Open fail."), FALSE);

    // CFile::Remove(pszFileName);
#else

    if (NULL == hWnd)
    {
        hWnd = GetDesktopWindow();
    }
    
    CRect rect; 
    CDC *pScrDc = IcyGetDCAndRect(hWnd, rect, bClient);
    
    CDC MemDc;
    MemDc.CreateCompatibleDC(pScrDc); 
    
    CBitmap tScrBmp;
    tScrBmp.CreateCompatibleBitmap(pScrDc, rect.Width(), rect.Height());
    
    CBitmap *ptOldBmp = MemDc.SelectObject(&tScrBmp);
    MemDc.BitBlt(0, 0, rect.Width(), rect.Height(), pScrDc, 0, 0, SRCCOPY);

    BITMAP bmp;
    tScrBmp.GetBitmap(&bmp);

    BITMAPINFOHEADER bih = {0};                                              // λͼ��Ϣͷ
    bih.biBitCount = bmp.bmBitsPixel;                                        // ÿ�������ֽڴ�С
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;                                             // �߶�
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // ͼ�����ݴ�С
    bih.biWidth = bmp.bmWidth; 

    // ΪDIB�����ڴ�
    m_hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
    if (m_hDIB == 0)
    {
        // �ڴ����ʧ�ܣ�����NULL��
        return NULL;
	}

    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //�����ڴ汣��λͼ����
    
    GetDIBits(MemDc.m_hDC, (HBITMAP) tScrBmp.m_hObject, 0, rect.Height(), p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);   

    MemDc.SelectObject(ptOldBmp);
    ::ReleaseDC(hWnd, pScrDc->m_hDC);

#endif

    return TRUE;
}


BOOL CIcyBitmap::InitInfo()
{
    ICY_ASSET_UNRET0(m_hDIB != NULL);   
    
    // ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_lpDIBHdr;

    // ָ��BITMAPCOREINFO�ṹ��ָ��
    LPBITMAPCOREHEADER lpbmc = (LPBITMAPCOREHEADER)m_lpDIBHdr;
    
    // ����DIB��ͼ��Ŀ��
    if (IS_WIN30_DIB(m_lpDIBHdr))
    {
        // ����Windows 3.0 DIB������lpbmi->biHeight
        m_dwHeight = lpbmi->biHeight;
        m_dwWidth = lpbmi->biWidth;
        m_byColorBits = lpbmi->biBitCount;
    }
    else
    {
        // ����������ʽ��DIB������lpbmc->bcHeight
        m_dwHeight = (DWORD)lpbmc->bcHeight;
        m_dwWidth = (DWORD)lpbmc->bcWidth;
        m_byColorBits =  (BYTE)lpbmc->bcBitCount; 
    }

    ICY_ASSET_RET_LOG(m_byColorBits > 16, _T("�����ɫBMP,��֧�ִ���"), FALSE);
    
    return TRUE;
    
}


BOOL CIcyBitmap::Save(LPCTSTR lpszPathName) 
{
    CFile file;
    CFileException fe;
    
    // ���ļ�
    if (!file.Open(lpszPathName, CFile::modeCreate |
        CFile::modeReadWrite | CFile::shareExclusive, &fe))
    {
        return FALSE;
    }
    
    // ���Ե���SaveDIB����ͼ��
    BOOL bSuccess = SaveDIB(m_hDIB, file);    

    if (!bSuccess)
    {       
        // ��ʾ����
        MessageBox(NULL, _T("�޷�����BMPͼ��"), "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
    }

    // �ر��ļ�
    file.Close();
    
    return bSuccess;
}

BOOL CIcyBitmap::Save()
{
   return Save(m_strPathName.GetBuffer(256));
}


DWORD CIcyBitmap::GetInfo(DWORD &dwWidth, DWORD &dwHeight,BYTE &byBitCount)
{   
    ICY_ASSET_RET(m_hDIB != NULL, 0);

    dwWidth = m_dwWidth ;
    dwHeight = m_dwHeight ;
    byBitCount = m_byColorBits ;

    return 0;
}


COLORREF CIcyBitmap::Color(INT x, INT y)
{

    // ����ͼ��ÿ�е��ֽ���
	// LONG lLineBytes = WIDTHBYTES(m_dwWidth * m_byColorBits);

    //Ҳ����������㷨
    LONG lLineBytes = (((m_dwWidth * m_byColorBits) + 31) >> 5) << 2;

    BYTE byBytesPerPoint = m_byColorBits/8;

    DWORD dwOffset = ((m_dwHeight-y-1) * lLineBytes) + (x * byBytesPerPoint);   
    
    // �ڴ�����RGB B�ڵ͵�ַ B G R A
    COLORREF color = RGB(m_lpDIBBits[dwOffset+2], m_lpDIBBits[dwOffset+1], m_lpDIBBits[dwOffset]);

    return color;
}

BOOL CIcyBitmap::FindColor(COLORREF color, 
                           INT left1, INT top1, 
                           INT left2, INT top2, 
                           INT &x, INT &y)
{

    if ((left1 > m_dwWidth || left2 > m_dwWidth)
        || (top1 > m_dwHeight || top2 > m_dwHeight))
    {
        ICY_ASSET_RET_LOG(FALSE, _T("FindColor::Invalid para"), FALSE);
    }

    if (left2 <= left1
        || top2 <= top1)
    {
        ICY_ASSET_RET_LOG(FALSE, _T("FindColor::Invalid para"), FALSE);
    }


    for (INT i = left1; i <= left2; i++)
    {
        for (INT j = top1; j <= top2; j++)
        {
            if (color == Color(i,j))
            {
                x = i;
                y = j;
                return TRUE;
            }
        }
    }

    return FALSE;

}


BOOL CIcyBitmap::FindBmp(int x1,int y1,int x2,int y2,
                         LPCTSTR lpszName,int &fx,int &fy)
{
    int iMatchTimes = ::FindBmp(x1,y1,x2,y2,
                                lpszName,
                                fx,fy);

    return (iMatchTimes > 0 ? TRUE : FALSE);

}

BOOL CIcyBitmap::FindBmpEx(int x1, int y1, int x2, int y2, LPCTSTR lpszName,
                          int backcolor,int errorcolor,int errorcount,
                          int &fx,int &fy)
{
    int iMatchTimes = ::FindBmpEx(x1,y1,x2,y2,
                                lpszName,
                                backcolor, errorcolor, errorcount,
                                fx,fy);
    
    return (iMatchTimes > 0 ? TRUE : FALSE);
}

