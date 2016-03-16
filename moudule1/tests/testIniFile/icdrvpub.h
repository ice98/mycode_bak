/*****************************************************************
****
** �ļ���:  drv_pub.h
** ��Ȩ:    ��������ͨѶ�ɷ����޹�˾����ҵ�����������
** ������:	����
** ����:    2005-03-15
** �޸���:  
** ����:     
** ����:    оƬ��д�����궨��
** �汾:    V1.00
*****************************************************************/
#ifndef CSS_IC_DRV_PUB_H
#define CSS_IC_DRV_PUB_H

#include "cssvar.h"

#ifdef _WIN32
#include "IniFile.h"
#endif

/***************************************************************
// �Ĵ���λ�Ĳ����궨��
*****************************************************************/
#define		DRV_BIT0      0x01      /*(1<<0)*/
#define		DRV_BIT1      0x02      /*(1<<1)*/
#define 	DRV_BIT2      0x04      /*(1<<2)*/
#define 	DRV_BIT3      0x08      /*(1<<3)*/
#define 	DRV_BIT4      0x10      /*(1<<4)*/
#define 	DRV_BIT5      0x20      /*(1<<5)*/
#define 	DRV_BIT6      0x40      /*(1<<6)*/
#define 	DRV_BIT7      0x80      /*(1<<7)*/
#define 	DRV_BIT8      0x100     /*(1<<8)*/
#define 	DRV_BIT9      0x200     /*(1<<9)*/
#define 	DRV_BIT10     0x400     /*(1<<10)*/
#define 	DRV_BIT11     0x800     /*(1<<11)*/
#define 	DRV_BIT12     0x1000    /*(1<<12)*/
#define 	DRV_BIT13     0x2000    /*(1<<13)*/
#define 	DRV_BIT14     0x4000    /*(1<<14)*/
#define 	DRV_BIT15     0x8000    /*(1<<15)*/
#define 	DRV_BIT16     0x10000   /*(1<<16) */
#define 	DRV_BIT17     0x20000   /*(1<<17)*/
#define 	DRV_BIT18     0x40000   /*(1<<18)*/
#define 	DRV_BIT19     0x80000   /*(1<<19)*/
#define 	DRV_BIT20     0x100000  /*(1<<20)*/
#define 	DRV_BIT21     0x200000  /*(1<<21)*/
#define 	DRV_BIT22     0x400000  /*(1<<22)*/
#define 	DRV_BIT23     0x800000  /*(1<<23)*/
#define 	DRV_BIT24     0x1000000 /*(1<<24)*/
#define 	DRV_BIT25     0x2000000 /*(1<<25)*/
#define 	DRV_BIT26     0x4000000 /*(1<<26)*/
#define 	DRV_BIT27     0x8000000 /*(1<<27)*/
#define 	DRV_BIT28     0x10000000/*(1<<28)*/
#define 	DRV_BIT29     0x20000000/*(1<<29)*/
#define 	DRV_BIT30     0x40000000/*(1<<30)*/
#define 	DRV_BIT31     0x80000000/*(1<<31)*/

/***************************************************************
// ����ICоƬ���ó�������
*****************************************************************/
#define DRV_IC_ENABLE_WRITECHIP  0x01
#define DRV_IC_DISABLE_WRITECHIP 0x00
/***************************************************************
// ƫ�Ƶ�ַ����λ�궨��
*****************************************************************/
#define DRV_OFFSET32(offsetaddr)  ((offsetaddr) << 2)
#define DRV_OFFSET16(offsetaddr)  ((offsetaddr) << 1)

/* ����Ĵ����Ķ�д�궨�� */
#ifdef WIN32 /* ��PC�Ϸ��棬���д�Ĵ����ͷŵ�INI�ļ���*/

#define  DRV_REG8(addr)  (BYTE)IniFileReadReg(addr, __FILE__)
#define  DRV_REG16(addr) (WORD)IniFileReadReg(addr, __FILE__)
#define  DRV_REG32(addr) (DWORD)IniFileReadReg(addr, __FILE__)

/***************************************************************
// ��WIN32�·��水���ֽ�����доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_BYTE1(addr, ucWrValue) \
  IniFileWriteReg(addr,(IniFileReadReg(addr, __FILE__)&0xffffff00)|(DWORD)(ucWrValue), __FILE__);

#define DRV_WRITE32_BYTE2(addr, ucWrValue) \
  IniFileWriteReg(addr,(IniFileReadReg(addr, __FILE__)&0xffff00ff)|(DWORD)(((DWORD)(ucWrValue))<<8), __FILE__)

#define DRV_WRITE32_BYTE3(addr, ucWrValue) \
  IniFileWriteReg(addr,(IniFileReadReg(addr, __FILE__)&0xff00ffff)|(DWORD)(((DWORD)(ucWrValue))<<16), __FILE__)

#define DRV_WRITE32_BYTE4(addr, ucWrValue) \
  IniFileWriteReg(addr,(IniFileReadReg(addr, __FILE__)&0x00ffffff)|(DWORD)(((DWORD)(ucWrValue))<<24), __FILE__)


#define DRV_READ32_BYTE1(addr) \
	((BYTE)(IniFileReadReg(addr, __FILE__)&0x000000ff))

#define DRV_READ32_BYTE2(addr) \
	((BYTE)((IniFileReadReg(addr, __FILE__)&0x0000ff00)>>8))

#define DRV_READ32_BYTE3(addr) \
	((BYTE)((IniFileReadReg(addr, __FILE__)&0x00ff0000)>>16))

#define DRV_READ32_BYTE4(addr) \
	((BYTE)((IniFileReadReg(addr, __FILE__)&0xff000000)>>24))

/***************************************************************
// ��WIN32�·��水��������доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_WORD1(addr, wWrValue) \
	IniFileWriteReg(addr,(WORD)((IniFileReadReg(addr, __FILE__)&0xffff0000)|(wWrValue)), __FILE__)

#define DRV_WRITE32_WORD2(addr, wWrValue) \
	IniFileWriteReg(addr,(IniFileReadReg(addr, __FILE__)&0x0000ffff)|(DWORD)(((DWORD)(wWrValue))<<16), __FILE__)

#define DRV_READ32_WORD1(addr) \
	((WORD)(IniFileReadReg(addr, __FILE__)&0x0000ffff));

#define DRV_READ32_WORD2(addr) \
	((WORD)((IniFileReadReg(addr, __FILE__)&0xffff0000)>>16));

/***************************************************************
// ��WIN32�·��水��˫������доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_DWORD(addr, dwWrValue) 	IniFileWriteReg(addr,(DWORD)(dwWrValue), __FILE__)
#define DRV_READ32_DWORD(addr) 	IniFileReadReg(addr, __FILE__)

/***************************************************************
// ��WIN32�·��水��λ����доƬ32λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG32BITX(addr, bBITX) \
	IniFileWriteReg(addr, IniFileReadReg(addr, __FILE__)|(DWORD)(1 << (bBITX)), __FILE__ )

#define DRV_CLR_REG32BITX(addr, bBITX) \
	IniFileWriteReg(addr, IniFileReadReg(addr, __FILE__)&(~(DWORD)(1 << (bBITX))), __FILE__ )

#define DRV_GET_REG32BITX(addr, bBITX) \
	(BYTE)((IniFileReadReg(addr, __FILE__)>>(bBITX))&0x01)

/***************************************************************
// ��WIN32�·��水���ֽ�����доƬ16λ�ļĴ���
*****************************************************************/
#define DRV_WRITE16_BYTE1(addr, ucWrValue) \
	IniFileWriteReg(addr,(WORD)((IniFileReadReg(addr, __FILE__)&0xff00)|(ucWrValue)), __FILE__)

#define DRV_WRITE16_BYTE2(addr, ucWrValue) \
	IniFileWriteReg(addr,(WORD)((IniFileReadReg(addr, __FILE__)&0x00ff)|(ucWrValue<<8)), __FILE__)

#define DRV_READ16_BYTE1(addr) \
	(BYTE)(IniFileReadReg(addr,__FILE__)&0x00ff)

#define DRV_READ16_BYTE2(addr) \
	(BYTE)((IniFileReadReg(addr, __FILE__)&0xff00)>>8)

/***************************************************************
// ��WIN32�·��水��������доƬ16λ�ļĴ���
*****************************************************************/
#define DRV_WRITE16_WORD(addr, wWrValue) \
	IniFileWriteReg(addr,(WORD)(wWrValue), __FILE__)

#define DRV_READ16_WORD(addr) \
	(WORD)IniFileReadReg(addr, __FILE__)

/***************************************************************
// ��WIN32�·��水��λ����доƬ16λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG16BITX(addr, bBITX) \
	IniFileWriteReg(addr, IniFileReadReg(addr, __FILE__)|(WORD)(1 << (bBITX)), __FILE__)

#define DRV_CLR_REG16BITX(addr, bBITX) \
	IniFileWriteReg(addr, IniFileReadReg(addr, __FILE__) &(~(WORD)(1 << (bBITX))), __FILE__)
	
#define DRV_GET_REG16BITX(addr, bBITX) \
	((IniFileReadReg(addr,__FILE__)>>(bBITX))&0x01)

/***************************************************************
// ��WIN32�·��水���ֽ�����доƬ8λ�ļĴ���
*****************************************************************/
#define DRV_WRITE8_BYTE(addr, ucWrValue) IniFileWriteReg(addr,(BYTE)ucWrValue, __FILE__)

#define DRV_READ8_BYTE(addr)  ((BYTE)IniFileReadReg(addr, __FILE__))

/***************************************************************
// ��WIN32�·��水��λ����доƬ8λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG8BITX(addr, bBITX) \
	IniFileWriteReg(addr, (BYTE)(IniFileReadReg(addr, __FILE__)|(BYTE)(1 << (bBITX))), __FILE__)

#define DRV_CLR_REG8BITX(addr, bBITX) \
	IniFileWriteReg(addr, (BYTE)(IniFileReadReg(addr, __FILE__) &(~(BYTE)(1 << (bBITX)))), __FILE__)

#define DRV_GET_REG8BITX(addr, bBITX) \
	((IniFileReadReg(addr, __FILE__)>>(bBITX))&0x01)
#else
/***************************************************************************************/
/***************************************************************
// Ӳ����Χ�Ĵ�����ַ�궨��
*****************************************************************/
#define  DRV_REG8(addr)  (*((volatile UCHAR  *)(addr)))
#define  DRV_REG16(addr) (*((volatile USHORT *)(addr)))
#define  DRV_REG32(addr) (*((volatile ULONG  *)(addr)))
/***************************************************************
// Ӳ����Χ�Ĵ�����ַ�궨��
*****************************************************************/
#define  DRV_REG8PTR(addr)  ((volatile UCHAR  *)(addr))
#define  DRV_REG16PTR(addr) ((volatile USHORT *)(addr))
#define  DRV_REG32PTR(addr) ((volatile ULONG  *)(addr))

/***************************************************************
// �����ֽ�����доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_BYTE1(addr, ucWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0xffffff00)|(DWORD)(ucWrValue))

#define DRV_WRITE32_BYTE2(addr, ucWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0xffff00ff)|(DWORD)(((DWORD)(ucWrValue))<<8))

#define DRV_WRITE32_BYTE3(addr, ucWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0xff00ffff)|(DWORD)(((DWORD)(ucWrValue))<<16))

#define DRV_WRITE32_BYTE4(addr, ucWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0x00ffffff)|(DWORD)(((DWORD)(ucWrValue))<<24))

#define DRV_READ32_BYTE1(addr) \
	((BYTE)(DRV_REG32(addr)&0x000000ff))

#define DRV_READ32_BYTE2(addr) \
	((BYTE)((DRV_REG32(addr)&0x0000ff00)>>8))

#define DRV_READ32_BYTE3(addr) \
	((BYTE)((DRV_REG32(addr)&0x00ff0000)>>16))

#define DRV_READ32_BYTE4(addr) \
	((BYTE)((DRV_REG32(addr)&0xff000000)>>24))

/***************************************************************
// ����������доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_WORD1(addr, wWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0xffff0000)|(DWORD)(wWrValue))

#define DRV_WRITE32_WORD2(addr, wWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0x0000ffff)|(DWORD)(((DWORD)(wWrValue))<<16))

#define DRV_READ32_WORD1(addr) \
	((WORD)(DRV_REG32(addr)&0x0000ffff));

#define DRV_READ32_WORD2(addr) \
	((WORD)((DRV_REG32(addr)&0xffff0000)>>16));
/***************************************************************
// ����˫������доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_DWORD(addr, dwWrValue) 	(DRV_REG32(addr) = (DWORD)(dwWrValue))

#define DRV_READ32_DWORD(addr) \
	(DRV_REG32(addr))
/***************************************************************
// ����λ����доƬ32λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG32BITX(addr, bBITX) \
	((DRV_REG32(addr)) |= (DWORD)(1 << (bBITX)))

#define DRV_CLR_REG32BITX(addr, bBITX) \
	((DRV_REG32(addr)) &= ~(DWORD)(1 << (bBITX)))

#define DRV_GET_REG32BITX(addr, bBITX) \
	((BYTE)(((DRV_REG32(addr)>>(bBITX))&0x01)))

/***************************************************************
// �����ֽ�����доƬ16λ�ļĴ���
*****************************************************************/
#define DRV_WRITE16_BYTE1(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0xff00)|(WORD)(ucWrValue))

#define DRV_WRITE16_BYTE2(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0x00ff)|(WORD)(((WORD)(ucWrValue))<<8))

#define DRV_READ16_BYTE1(addr) \
	((BYTE)(DRV_REG16(addr)&0x00ff))

#define DRV_READ16_BYTE2(addr) \
	((BYTE)((DRV_REG16(addr)&0xff00)>>8))

/***************************************************************
// ����������доƬ16λ�ļĴ���
*****************************************************************/
#define DRV_WRITE16_WORD(addr, wWrValue) \
	(DRV_REG16(addr) = (WORD)(wWrValue))

#define DRV_READ16_WORD(addr) \
	((WORD)(DRV_REG16(addr)))

/***************************************************************
// ����λ����доƬ16λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG16BITX(addr, bBITX) \
	((DRV_REG16(addr)) |= (WORD)(1 << (bBITX)))

#define DRV_CLR_REG16BITX(addr, bBITX) \
	((DRV_REG16(addr)) &= ~(WORD)(1 << (bBITX)))

#define DRV_GET_REG16BITX(addr, bBITX) \
	((DRV_REG16(addr)>>(bBITX))&0x01)

/***************************************************************
// �����ֽ�����доƬ8λ�ļĴ���
*****************************************************************/
#define DRV_WRITE8_BYTE(addr, ucWrValue) (DRV_REG8(addr) = (ucWrValue))

#define DRV_READ8_BYTE(addr)  ((BYTE)(DRV_REG8(addr)))

/***************************************************************
// ����λ����доƬ8λ�ļĴ���
// bBITX - ���봫��ʮ���Ƶ�λ�����Ҵ��㿪ʼ���
***************************************************************/
#define DRV_SET_REG8BITX(addr, bBITX) \
	((DRV_REG8(addr)) |= (BYTE)(1 << (bBITX)))

#define DRV_CLR_REG8BITX(addr, bBITX) \
	((DRV_REG8(addr)) &= ~(BYTE)(1 << (bBITX)))

#define DRV_GET_REG8BITX(addr, bBITX) \
	((DRV_REG8(addr)>>(bBITX))&0x01)

/***************************************************************
// ���հ��ֽ�����доƬ16λ�ļĴ���
****************************************************************/
#define DRV_WRITE16_HALFBYTE1(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0xfff0)|(WORD)(ucWrValue&0x0f))

#define DRV_WRITE16_HALFBYTE2(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0xff0f)|(WORD)((ucWrValue&0x0f)<<4))

#define DRV_WRITE16_HALFBYTE3(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0xf0ff)|(WORD)((ucWrValue&0x0f)<<8))

#define DRV_WRITE16_HALFBYTE4(addr, ucWrValue) \
	(DRV_REG16(addr) = (DRV_REG16(addr)&0x0fff)|(WORD)((ucWrValue&0x0f)<<12))
/***************************************************************
// ���հ��ֽ�����доƬ32λ�ļĴ���
*****************************************************************/
#define DRV_WRITE32_HALFBYTE1(addr, ucWrValue) \
	(DRV_REG32(addr) = (DRV_REG32(addr)&0xfffffff0)|(DWORD)(ucWrValue&0x0f))

#define DRV_WRITE32_HALFBYTE2(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xffffff0f)|(DWORD)((ucWrValue&0x0f)<<4))

#define DRV_WRITE32_HALFBYTE3(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xfffff0ff)|(DWORD)((ucWrValue&0x0f)<<8))

#define DRV_WRITE32_HALFBYTE4(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xffff0fff)|(DWORD)((ucWrValue&0x0f)<<12))

#define DRV_WRITE32_HALFBYTE5(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xfff0ffff)|(DWORD)((ucWrValue&0x0f)<<16))

#define DRV_WRITE32_HALFBYTE6(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xff0fffff)|(DWORD)((ucWrValue&0x0f)<<20))

#define DRV_WRITE32_HALFBYTE7(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0xf0ffffff)|(DWORD)((ucWrValue&0x0f)<<24))

#define DRV_WRITE32_HALFBYTE8(addr, ucWrValue) \
	(DRV_REG32(addr) = 	(DRV_REG32(addr)&0x0fffffff)|(DWORD)((ucWrValue&0x0f)<<28))


#endif

/***************************************************************
// ��ӼĴ�����BUSY����Լ���ʱ����
// regaddr   - BUSYλ���ڵļĴ�����ַ
// busybit   - BUSYλ
// dwCnt     - ��ʱ�ļ���
// dwTimeOut - ��ʱʱ�䣬���մ���������
***************************************************************/
#define DRV_POLL_BUSY32(regaddr,busybit,busyendflag,dwCnt, dwTimeOut) \
{\
	for (dwCnt =0; dwCnt<= dwTimeOut; dwCnt++) \
	{  \
		if(DRV_GET_REG32BITX((regaddr),(busybit)) == (busyendflag))\
		{\
			    break;\
		}\
	}  \
}

#define DRV_POLL_BUSY16(regaddr,busybit,busyendflag,dwCnt, dwTimeOut) \
{\
	for (dwCnt =0; dwCnt<= dwTimeOut; dwCnt++) \
	{  \
		if(DRV_GET_REG16BITX((regaddr),(busybit)) == (busyendflag))\
		{\
			break;\
		}\
	}  \
}
#define DRV_POLL_BUSY8(regaddr,busybit,busyendflag,dwCnt, dwTimeOut) \
{\
	for (dwCnt =0; dwCnt<= dwTimeOut; dwCnt++)  \
	{  \
		if(DRV_GET_REG8BITX((regaddr),(busybit)) == (busyendflag))\
		{\
			break;\
		}\
	}  \
}


// ����ƽ̨�ӿڵ�ҵ��ӿڲ��Ա���ͷ����
#pragma  pack(1)
   
typedef struct 
{
    WORD 	wCmdCode;
    DWORD 	dwLength;
    WORD 	wItemNum;
    DWORD   dwOperator;
    BYTE    ucIsSetOrGet;
    DWORD 	dwReserved;
}T_DrvMgrInfTestHeader;
    

#pragma pack()


#endif
