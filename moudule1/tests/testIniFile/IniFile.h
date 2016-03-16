/*****************************************************************
****
** 文件名:  IniFile.h
** 版权:    深圳中兴通讯股份有限公司传输业务软件开发部
** 创建人:	刘林
** 日期:    2005-11-18
** 修改人:  
** 日期:    
** 描述:	
** 版本:    V1.0
*****************************************************************/
#ifdef _WIN32
#ifndef		__INI_FILE_SPORC_H_C
#define		__INI_FILE_SPORC_H_C

#include  "cssvar.h"
#include "csserror.h"


/************************************************************* 
// 配置模块的宏定义
*************************************************************/
/* Ini文件一行的最大长度 */
#define  INI_FILE_LINE_MAXCHAR_NUM  255

/************************************************************* 
// 外部接口函数
*************************************************************/

/* 初始化模块 */
/*DWORD IniFileLoadSimuFile(const char* sFileName);*/
/* 从INI文件中读取寄存器的值 */
DWORD IniFileReadReg(const DWORD dwRegAddr, const char* sIniFileRelPath);
/* 从INI文件中写寄存器的值 */
DWORD IniFileWriteReg(const DWORD dwRegAddr, const DWORD dwRegValue, const char* sIniFileRelPath);

DWORD CSS_IstHexStrToHex(const char *pBuffer);
					

#endif /*头文件定义结束*/
#endif