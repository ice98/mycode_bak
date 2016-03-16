#include "iostream.h"
char   *lpBufscr,*lpBufbmp;
BITMAP   bb,bm;//bb=��Ļ  bm=ͼƬ
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

//�ͷ�lpBuf
int ReleaseLP()
{
	HANDLE hp=GetProcessHeap();
	HeapFree(hp,0,lpBufscr);
	HeapFree(hp,0,lpBufbmp);
	return 0;
}


int GetScrPixel(int x,int y)//��ȡ��Ļ��ɫ
{

	if(x<0 || x>(int)dwX || x<0 || x>(int)dwY)return-1;
	BYTE   bBlue=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 0]; 
	BYTE   bGreen=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 1]; 
	BYTE   bRed=lpBufscr[bb.bmWidthBytes*(dwY-y-1) + bb.bmBitsPixel*x + 2]; 

	COLORREF   color=RGB(bBlue,bGreen,bRed); 
	return int(color);
}

int GetBmpPixel(int x,int y)//��ȡλͼ��ɫ
{
	if(x<0 || x>(int)dwX || x<0 || x>(int)dwY)return-1;
	BYTE   bBlue=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 0]; 
	BYTE   bGreen=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 1]; 
	BYTE   bRed=lpBufbmp[bm.bmWidthBytes*(bm.bmHeight-y-1) + bm.bmBitsPixel*x/8 + 2]; 

	COLORREF   color=RGB(bBlue,bGreen,bRed); 
	return int(color);
	//return lpBufbmp[1];
}


//���Ҿ�����������ɫֵΪ color�ĵ�
//������������������
//�ҵ�����1,�������긳ֵ��������fx��fy��,�Ҳ����򷵻�0
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


//�Ƚ���Ļpt��λͼ���δ�С��������ɫ��λ���Ƿ���λͼ��ͬ
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

//������char *lpBufscr�в���*lpBufbmp;
int FindBmpBuf(int x1,int y1,int x2,int y2,int &fx,int &fy)
{
	POINT pt,pttmp;
	fx=-1;
	fy=-1;

	pt=FindFirstPoint(x1,y1,x2,y2);
	if(Compare(pt)){fx=pt.x;fy=pt.y;return 1;}
	
	else//���ֵ�һ�����ҵ���ȴ������λͼ�Ǻ�
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

//�Ӿ���x1,y1,x2,y2�ڲ�����λͼlpszName��ȫƥ�����ʼ��
//���ҵ�,(fx,fy)ΪͼƬ��һ�γ��ֵ����ϽǶ���,������1
//���Ҳ���,(fx,fy)=(-1,-1)������0
int FindBmp(int x1,int y1,int x2,int y2,LPCTSTR lpszName,int &fx,int &fy)
{
	GetBmpData(lpszName);
	GetScreenData(x2,y2);

	FindBmpBuf(x1,y1,x2,y2,fx,fy);

	ReleaseLP();
	if(fx>0)return 1;
	return 0;

}

//�Ƚ���Ļpt��λͼ���δ�С��������ɫ��λ���Ƿ���λͼ����ƥ��
//λͼbackcolor��ɫ����ԣ���ɫƫ��errorcolor������ƥ���ĸ���errorcount
//�Ƚϳɹ�����1�����򷵻�0
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

//�Ӿ���x1,y1,x2,y2�ڲ�����λͼlpszName����ƥ�����ʼ��
//����ƥ���,(fx,fy)ΪͼƬ��һ�γ��ֵ����ϽǶ���,������ƥ�����
//����λͼbackcolor��ɫ�㣬��ɫƫ��errorcolor������ƥ���ĸ���errorcount
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