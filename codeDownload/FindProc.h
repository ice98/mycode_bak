#include "iostream.h"
char   *lpBufscr,*lpBufbmp;
BITMAP   bb,bm;//bb=屏幕  bm=图片
DWORD   dwX,dwY; 

struct Points
{
	POINT pts;
	struct Points *next;
};
void GetScreenData(int dx,int dy)
{

	HBITMAP   hBitmap,hOld;
	HDC   hDC,hcDC;
	BITMAPINFO   b;
	HANDLE   hp;

//	DWORD   dwX,dwY; 
	dwX=GetSystemMetrics(SM_CXSCREEN);
	dwY=GetSystemMetrics(SM_CYSCREEN); 
	dwX=min(dwX,(unsigned)dx);
	dwY=min(dwY,(unsigned)dy);
	hDC=::GetWindowDC(NULL); 
	hcDC=CreateCompatibleDC(hDC);
	hBitmap=CreateCompatibleBitmap(hDC,dwX,dwY);
	hOld=(HBITMAP)SelectObject(hcDC,hBitmap);
	BitBlt(hcDC,0,0,dwX,dwY,hDC,0,0,SRCCOPY);

	bb.bmWidth=dwX;
	bb.bmHeight=dwY;
	bb.bmPlanes=1;
	bb.bmWidthBytes=bb.bmWidth*3;
	bb.bmBitsPixel=3;
	bb.bmType=0;
	b.bmiHeader.biSize   =   sizeof(BITMAPINFOHEADER);
	b.bmiHeader.biWidth=dwX;
	b.bmiHeader.biHeight=dwY;
	b.bmiHeader.biPlanes   =   1;
	b.bmiHeader.biBitCount   =24;
	b.bmiHeader.biCompression   =   BI_RGB;
	b.bmiHeader.biSizeImage   =   0;
	b.bmiHeader.biXPelsPerMeter   =   0;
	b.bmiHeader.biYPelsPerMeter   =   0;
	b.bmiHeader.biClrUsed   =   0;
	b.bmiHeader.biClrImportant   =   0;
	b.bmiColors[0].rgbBlue=8;
	b.bmiColors[0].rgbGreen=8;
	b.bmiColors[0].rgbRed=8;
	b.bmiColors[0].rgbReserved=0;

	hp=GetProcessHeap();
	lpBufscr=(char *)HeapAlloc(hp,HEAP_ZERO_MEMORY,bb.bmHeight*bb.bmWidth*4);
	GetDIBits(hcDC,hBitmap,0,dwY,lpBufscr,&b,DIB_RGB_COLORS);

	ReleaseDC(NULL,hDC);   
	DeleteDC(hcDC);   
	DeleteObject(hBitmap);   
	DeleteObject(hOld);   
//	HeapFree(hp,0,lpBufscr);

}

void GetBmpData(LPCTSTR lpszName)
{
	HANDLE hp;
	HDC hdc,hdcmem;
	HBITMAP hBm;
	BITMAPINFO   b;
	hBm=(HBITMAP)LoadImage(NULL,/*"c:\\1.bmp"*/lpszName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	GetObject(hBm,sizeof(BITMAP),(LPVOID)&bm);
	hdc=::GetWindowDC(NULL);
	hdcmem=CreateCompatibleDC(hdc);
	SelectObject(hdcmem,hBm); 
	BitBlt(hdcmem,0,0,bm.bmWidth,bm.bmHeight,hdcmem,0,0,SRCCOPY);

	b.bmiHeader.biSize   =   sizeof(BITMAPINFOHEADER);
	b.bmiHeader.biWidth=bm.bmWidth;
	b.bmiHeader.biHeight=bm.bmHeight;
	b.bmiHeader.biPlanes   =   1;
	b.bmiHeader.biBitCount   =24;
	b.bmiHeader.biCompression   =   BI_RGB;
	b.bmiHeader.biSizeImage   =   0;
	b.bmiHeader.biXPelsPerMeter   =   0;
	b.bmiHeader.biYPelsPerMeter   =   0;
	b.bmiHeader.biClrUsed   =   0;
	b.bmiHeader.biClrImportant   =   0;
	b.bmiColors[0].rgbBlue=8;
	b.bmiColors[0].rgbGreen=8;
	b.bmiColors[0].rgbRed=8;
	b.bmiColors[0].rgbReserved=0;

	hp=GetProcessHeap();
	lpBufbmp=(char *)HeapAlloc(hp,HEAP_ZERO_MEMORY,bm.bmHeight*bm.bmWidth*4);
	GetDIBits(hdc,hBm,0,bm.bmHeight,lpBufbmp,&b,DIB_RGB_COLORS);

	DeleteObject(&hBm);
	DeleteDC(hdc);
	DeleteDC(hdcmem);
}

//释放lpBuf
int ReleaseLP()
{
	HANDLE hp=GetProcessHeap();
	HeapFree(hp,0,lpBufscr);
	HeapFree(hp,0,lpBufbmp);
	return 0;
}


int GetScrPixel(int x,int y)//获取屏幕颜色
{

	if(x<0 || x>(int)dwX || x<0 || x>(int)dwY)return-1;
	BYTE   bBlue=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 0]; 
	BYTE   bGreen=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 1]; 
	BYTE   bRed=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 2]; 

	COLORREF   color=RGB(bBlue,bGreen,bRed); 
	return int(color);
}

int GetBmpPixel(int x,int y)//获取位图颜色
{
	if(x<0 || x>(int)dwX || x<0 || x>(int)dwY)return-1;
	BYTE   bBlue=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 0]; 
	BYTE   bGreen=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 1]; 
	BYTE   bRed=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 2]; 

	COLORREF   color=RGB(bBlue,bGreen,bRed); 
	return int(color);
	//return lpBufbmp[1];
}


//查找矩形区域内颜色值为 color的点
//从左往右逐行往下找
//找到返回1,并将坐标赋值给参数（fx，fy）,找不到则返回0
int FindColor(int x1,int y1,int x2,int y2,int color,int &fx,int &fy)
{
	GetScreenData(x2,y2);
	fx=fy=-1;
	int xtmp=x1;
	for(;y1<y2;y1++)
	{
		for(x1=xtmp;x1<=x2;x1++)
			{
				if(GetScrPixel(x1,y1)==color)
				{
					fx=x1;
					fy=y1;
					ReleaseLP();
					return 1;
				}
			}
	}
	ReleaseLP();
	return 0;
}

POINT FindFirstPoint(int x1,int y1,int x2,int y2)
{
	int color=GetBmpPixel(0,0);
	POINT pt;
	pt.x=-1;
	pt.y=-1;
	int fx,fy,xtmp=x1;;
	fx=fy=-1;
	for(;y1<y2;y1++)
		for(x1=xtmp;x1<x2;x1++)
			{
				if(GetScrPixel(x1,y1)==color)
				{
					pt.x=x1;
					pt.y=y1;
					return pt;
				}
			}
	return pt;
}


//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图相同
int Compare(POINT pt)
{
	int x,y;
	if((unsigned)bm.bmHeight>dwY-pt.y)return 0;
	for(y=0;y<bm.bmHeight-1;y++)
	{
		for(x=0;x<bm.bmWidth-1;x++)
		{
			if(GetBmpPixel(x,y)==GetScrPixel(pt.x+x,pt.y+y))
			{
			
			
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

//从数据char *lpBufscr中查找*lpBufbmp;
int FindBmpBuf(int x1,int y1,int x2,int y2,int &fx,int &fy)
{
	POINT pt,pttmp;
	fx=-1;
	fy=-1;

	pt=FindFirstPoint(x1,y1,x2,y2);
	if(Compare(pt)){fx=pt.x;fy=pt.y;return 1;}
	
	else//出现第一个点找到后却并不与位图吻合
		{
			pttmp.y=pt.y+1;
			while(pt=FindFirstPoint(pt.x+1,pt.y,x2,pt.y+1) ,pt.x>=0)
			{
				if(Compare(pt)){fx=pt.x;fy=pt.y;return 1;}
			}
			while(pt=FindFirstPoint(x1,pttmp.y,x2,y2) ,pt.x>=0)
			{
				if(Compare(pt)){fx=pt.x;fy=pt.y;return 1;}
				else
				{
					pttmp.y=pt.y+1;
					while(pt=FindFirstPoint(pt.x+1,pt.y,x2,pt.y+1) ,pt.x>=0)
						if(Compare(pt)){fx=pt.x;fy=pt.y;return 1;}
				}
			}
		}
	return 0;
}

//从矩形x1,y1,x2,y2内查找与位图lpszName完全匹配的起始点
//若找到,(fx,fy)为图片第一次出现的左上角顶点,并返回1
//若找不到,(fx,fy)=(-1,-1)并返回0
int FindBmp(int x1,int y1,int x2,int y2,LPCTSTR lpszName,int &fx,int &fy)
{
	GetBmpData(lpszName);
	GetScreenData(x2,y2);

	FindBmpBuf(x1,y1,x2,y2,fx,fy);

	ReleaseLP();
	if(fx>0)return 1;
	return 0;

}

//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图条件匹配
//位图backcolor颜色点忽略，颜色偏差errorcolor，允许不匹配点的个数errorcount
//比较成功返回1，否则返回0
int CompareEx(POINT pt,int backcolor,int errorcolor,int errorcount)
{
	int x,y,colorB,colorS,count=0;
	if((unsigned)bm.bmHeight>dwY-pt.y)return 0;
	for(y=0;y<bm.bmHeight-1;y++)
		for(x=0;x<bm.bmWidth-1;x++)
		{
			colorB=GetBmpPixel(x,y);
			colorS=GetScrPixel(pt.x+x,pt.y+y);
			if(colorB==colorS || colorB==backcolor);
			else
			{
				if( abs(GetRValue((COLORREF)colorB)-GetRValue((COLORREF)colorS) > GetRValue((COLORREF)errorcolor))
					|| abs(GetGValue((COLORREF)colorB)-GetGValue((COLORREF)colorS) > GetGValue((COLORREF)errorcolor))
					|| abs(GetBValue((COLORREF)colorB)-GetBValue((COLORREF)colorS) > GetBValue((COLORREF)errorcolor)) ){
					count++;
				}
			}
		}
	if(count>errorcount)
		return 0;
	else
		return 1;
}

//从矩形x1,y1,x2,y2内查找与位图lpszName条件匹配的起始点
//若有匹配的,(fx,fy)为图片第一次出现的左上角顶点,并返回匹配个数
//忽略位图backcolor颜色点，颜色偏差errorcolor，允许不匹配点的个数errorcount
int FindBmpEx(int x1,int y1,int x2,int y2,LPCTSTR lpszName,int backcolor,int errorcolor,int errorcount,int &fx,int &fy)
{
	int x,y,color=-1;

	GetBmpData(lpszName);
	GetScreenData(x2,y2);
	for(y=0;y<bm.bmHeight-1;y++)
	{
		for(x=0;x<bm.bmWidth-1;x++)
			if(GetBmpPixel(x,y)!=errorcount)
			{
				color=GetBmpPixel(x,y);
				break;
			}
		if(color>0)break;
	}

	Points *head,*tail,*p,*q;
	head=tail=NULL;
	int xtmp=x1,countP=0,countB=0;
	for(;y1<y2;y1++)
		for(x1=xtmp;x1<=x2;x1++)
			if(GetScrPixel(x1,y1)==color)
				{
					countP++;
					p=(struct Points *)malloc(sizeof(struct Points));
					(p->pts).x=x1-x;
					(p->pts).y=y1-y;
					if(head==NULL)q=head=tail=p;
					else
						tail=tail->next;
					tail->next=p;
				}
	p->next=NULL;
	while(q)
	{
		if(CompareEx((q->pts),backcolor,errorcolor,errorcount))
		{
			countB++;
			if(countB==1)
			{
				fx=(q->pts).x;
				fy=(q->pts).y;
			}
		}
		p=q->next;
		free(q);
		q=p;
	}

	ReleaseLP();
	return countB;

}