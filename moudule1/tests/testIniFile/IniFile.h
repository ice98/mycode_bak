/*****************************************************************
****
** �ļ���:  IniFile.h
** ��Ȩ:    ��������ͨѶ�ɷ����޹�˾����ҵ�����������
** ������:	����
** ����:    2005-11-18
** �޸���:  
** ����:    
** ����:	
** �汾:    V1.0
*****************************************************************/
#ifdef _WIN32
#ifndef		__INI_FILE_SPORC_H_C
#define		__INI_FILE_SPORC_H_C

#include  "cssvar.h"
#include "csserror.h"


/************************************************************* 
// ����ģ��ĺ궨��
*************************************************************/
/* Ini�ļ�һ�е���󳤶� */
#define  INI_FILE_LINE_MAXCHAR_NUM  255

/************************************************************* 
// �ⲿ�ӿں���
*************************************************************/

/* ��ʼ��ģ�� */
/*DWORD IniFileLoadSimuFile(const char* sFileName);*/
/* ��INI�ļ��ж�ȡ�Ĵ�����ֵ */
DWORD IniFileReadReg(const DWORD dwRegAddr, const char* sIniFileRelPath);
/* ��INI�ļ���д�Ĵ�����ֵ */
DWORD IniFileWriteReg(const DWORD dwRegAddr, const DWORD dwRegValue, const char* sIniFileRelPath);

DWORD CSS_IstHexStrToHex(const char *pBuffer);
					

#endif /*ͷ�ļ��������*/
#endif