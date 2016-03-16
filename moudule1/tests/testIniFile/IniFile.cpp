/*****************************************************************
****
** 文件名:  Inifile.c
** 版权:    深圳中兴通讯股份有限公司传输
** 创建人:	刘林
** 日期:    2005-11-18
** 修改人:  
** 日期:    
** 描述:	
** 版本:    V1.0
*****************************************************************/
#ifdef _WIN32
#include "cssvar.h"
#include "csserror.h"

#include  "stdio.h"
#include  "string.h"
#include  "stdlib.h"
#include  "IniFile.h"
#include  "windows.h"

/*****************************************************************
//  内部函数定义
*****************************************************************/
#define ERR_INVALID_PARA     0x01
#define ERR_MEM_POINTER_NULL 0x02
HERROR CSS_IstFormat(char *pDesStr, const char *pSrcStr);


static DWORD IniFileConvToIniFileName(const char* sIniFileRelPath, char* sIniFileName);
static DWORD IniFileParseParamter(const char* sLineText);
static DWORD IniFileIndirect_WR(const char* sRegAddr, const DWORD dwRegValue,const char* sIniFileName);

static DWORD IniFileWrite_Trans_way1(const char* sRegAddr, const DWORD dwRegValue,
						 		     const char* sIniFileName);
static DWORD IniFileWrite_Trans_way2(const char* sRegAddr, const DWORD dwRegValue,
									 const char* sIniFileName);

/*****************************************************************
//  内部全局变量的定义
*****************************************************************/
/* 保存文件名 */
/*static char g_sNameFile[INI_FILE_LINE_MAXCHAR_NUM];*/
/* 保存寄存器的SECTION名称 */
static const char* g_psRegSectionName = "ICChip_SimuRegister";
static char  g_sParaSovle[INI_FILE_LINE_MAXCHAR_NUM][INI_FILE_LINE_MAXCHAR_NUM];



/* 定义寄存器属性的段名称*/
static const char* g_psRegProSecName = "REG_PROPERTY";
/* 定义寄存器的属性 */
static const char* g_psRegProKey_ONLY_READ   = "ONLY_READ";
static const char* g_psRegProKey_READ_CLEAR  = "READ_CLEAR";
static const char* g_psRegProKey_WRITE_CLEAR = "WRITE_CLEAR";
// 写转移型寄存器属性
static const char* g_psRegProKey_WRITE_TRANS1 = "WRITE_TRANS_WAY1";
static const char* g_psRegProKey_WRITE_TRANS2 = "WRITE_TRANS_WAY2";

// 写转移型寄存器，当写入不同的值，则寄存器的值自动变化
static const char* g_psRegProKey_WRITE_TRANS3 = "WRITE_TRANS_WAY3";

// 间接读写取芯片的值（1个控制寄存器，1个数据寄存器的模式）
static const char* g_psRegProKey_INDIRECT_WR = "INDIRECT_WR";



// 绑定寄存器的定义，在写操作时，将对等号左右的寄存器同时操作，相等关联。
static const char* g_psRegBindSecName = "REG_EQU_BIND";

// 写转移段名称
// 这种方式表示直接转移到写的这个寄存器中
static const char* g_psRegWTransSecName1 = "REG_WRITE_TRANS_WAY1";
// 这种方式表示直接转移到其他的寄存器中
static const char* g_psRegWTransSecName2 = "REG_WRITE_TRANS_WAY2";
// 间接读取的段名称
static const char* g_psRegINDIRECT_WRSecName = "REG_INDIRECT_WR";



/*****************************************************************
****
** 函数名:      IniFileReadReg()
** 输  入:      读取的寄存器地址
** 输  出:      无
** 描  述:		读取指定地址的寄存器值
			    寄存器地址和寄存器内保存的值都是十六进制表示的，
				属性字段为"WR", "R", 用于寄存器的硬件特性，
				"WR"为可读可写， "R"-为只读
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-11-18 
** 修    改:      
** 日    期:
*****************************************************************/
DWORD IniFileReadReg(const DWORD dwRegAddr,const char* sIniFileRelPath)
{
	const char* pErrInfo = "Error: To Read Register failed";

	char sRegAddr[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegAddrProt[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegValue[INI_FILE_LINE_MAXCHAR_NUM] = {0};
	char sRegProt[INI_FILE_LINE_MAXCHAR_NUM] = {0};

	DWORD dwRegValue = 0;

	char sIniFileName[INI_FILE_LINE_MAXCHAR_NUM] = {0};
	
	/*将寄存器地址转成为字符串*/
	sprintf(sRegAddr,"%x", dwRegAddr);

	IniFileConvToIniFileName(sIniFileRelPath,sIniFileName);

	/* 读寄存器 */
	GetPrivateProfileString(g_psRegSectionName, 
                            sRegAddr, 
                            pErrInfo, 
                            sRegValue, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName); 
	if(strcmp(sRegValue,pErrInfo) == 0)
	{
		dwRegValue = 0;	
	}
	else
	{
		dwRegValue = CSS_IstHexStrToHex(sRegValue);
	}

	strcpy(sRegAddrProt, sRegAddr);
	strcat(sRegAddrProt,"str");

	/* 读寄存器的属性，根据属性决定下一步操作 */
	GetPrivateProfileString(g_psRegProSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegProt, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);

	/* 查询到了寄存器属性设置 */
	if(strcmp(pErrInfo, sRegProt) != 0)
	{
		/* 如果是读后清零，则写零到寄存器中*/
		if(strstr(sRegProt, g_psRegProKey_READ_CLEAR) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegAddr,"0",sIniFileName);
		}
		
	}

	return dwRegValue;
}

/*****************************************************************
****
** 函数名:      IniFileWriteReg()
** 输  入:      
** 输  出:      无
** 描  述:		读取指定地址的寄存器值
			    寄存器地址和寄存器内保存的值都是十六进制表示的，
				属性字段为"WR", "R", 用于寄存器的硬件特性，
				"WR"为可读可写， "R"-为只读
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-11-19 
** 修    改:      
** 日    期:
*****************************************************************/
DWORD IniFileWriteReg(const DWORD dwRegAddr, const DWORD dwRegValue,const char* sIniFileRelPath)
{
	const char* pErrInfo = "Error: To Read Register failed";
	char sRegProt[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegAddr[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegAddrProt[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegValue[INI_FILE_LINE_MAXCHAR_NUM] = {0};
	char sRegBind[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
//	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
//	char sRegTemp1[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
//	char sTransControl[INI_FILE_LINE_MAXCHAR_NUM]  = {0};

//	DWORD dwParaSum = 0;
	DWORD i = 0;

//	DWORD dwChannelNo = 0;
//	DWORD dwRegMask = 0;



	char sIniFileName[INI_FILE_LINE_MAXCHAR_NUM] = {0};
	

	// 将寄存器地址转成为字符串
	sprintf(sRegAddr,"%x",  dwRegAddr);
	sprintf(sRegValue,"%x", dwRegValue);

	IniFileConvToIniFileName(sIniFileRelPath,sIniFileName);
	
	for(i = 0; i < strlen(sRegValue); i++)
	{
		sRegValue[i] = toupper(sRegValue[i]);
	}

	strcpy(sRegAddrProt, sRegAddr);
	strcat(sRegAddrProt,"str");
	/* 读寄存器的属性，根据属性决定下一步操作 */
	GetPrivateProfileString(g_psRegProSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegProt, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);

	/* 根据关联寄存器操作，写数据到相关联的寄存器中 */
	GetPrivateProfileString(g_psRegBindSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegBind, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);
	/* 查询到了寄存器属性设置 */
	if(strcmp(pErrInfo, sRegProt) != 0)
	{
		/* 如果是写后清零，则写零到寄存器中*/
		if(strstr(sRegProt, g_psRegProKey_READ_CLEAR) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegAddr,"0",sIniFileName);
			/* 如果存在绑定寄存器操作则执行*/
			if(strcmp(pErrInfo, sRegBind) != 0)
			{
				WritePrivateProfileString(g_psRegSectionName,sRegBind,"0",sIniFileName);
			}
		}
		else if(strstr(sRegProt, g_psRegProKey_WRITE_TRANS1) != 0) // 写转移型操作方式1
		{
			IniFileWrite_Trans_way1(sRegAddr, dwRegValue, sIniFileName);
		}		
		else if(strstr(sRegProt, g_psRegProKey_WRITE_TRANS2) != 0) // 写转移型操作方式2
		{
			IniFileWrite_Trans_way2(sRegAddr, dwRegValue, sIniFileName);
		}
		else if(strstr(sRegProt, g_psRegProKey_INDIRECT_WR) != 0) // 间接读取
		{

			IniFileIndirect_WR(sRegAddr, dwRegValue, sIniFileName);
		}
		else if(strstr(sRegProt, g_psRegProKey_ONLY_READ) != 0) // 只读寄存器不能写入
		{
			;
		}
		else
		{
			;
		}
	}
	else // 可读可写的普通寄存器
	{
		WritePrivateProfileString(g_psRegSectionName, 
								sRegAddr, 
								sRegValue, 
								sIniFileName); 
		/* 如果存在绑定寄存器操作则执行*/
		if(strcmp(pErrInfo, sRegBind) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegBind,sRegValue,sIniFileName);
		}
	}


		
	return ERR_SUCCESS;
}
/*****************************************************************
****
** 函数名:      IniFileWrite_Trans_way1()
** 输  入:      需要解析的源头
** 输  出:      无
** 描  述:		
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-12-13
** 修    改:      
** 日    期:
*****************************************************************/
static DWORD IniFileWrite_Trans_way1(const char* sRegAddr, const DWORD dwRegValue,
						 		  const char* sIniFileName)
{
	const char* pErrInfo = "Error: To Read Register failed";
	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
		
	// 查询转移字段的相关参数
	GetPrivateProfileString(g_psRegWTransSecName1, 
                            sRegAddr, 
                            pErrInfo, 
                            sRegTemp, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);
			
	if(strcmp(pErrInfo, sRegTemp) == 0)
	{
		strcpy(sRegTemp,"0");
	}

	WritePrivateProfileString(g_psRegSectionName,sRegAddr,sRegTemp,sIniFileName);

	return ERR_SUCCESS;
}
/*****************************************************************
****
** 函数名:      IniFileWrite_Trans_way2()
** 输  入:      需要解析的源头
** 输  出:      无
** 描  述:		以','为分割点解析参数
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-12-13
** 修    改:      
** 日    期:
*****************************************************************/
static DWORD IniFileWrite_Trans_way2(const char* sRegAddr, const DWORD dwRegValue,
									 const char* sIniFileName)
									 
{
#if 0
// 	const char* pErrInfo = "Error: To Read Register failed";
// 	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
// 	char sRegTemp1[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
// 
// 	// 查询转移字段的相关参数
// 	GetPrivateProfileString(g_psRegWTransSecName2, 
// 		sRegAddr, 
// 		pErrInfo, 
// 		sRegTemp, 
// 		INI_FILE_LINE_MAXCHAR_NUM, 
// 		sIniFileName);
// 	
// 	if(strcmp(pErrInfo, sRegTemp) != 0)
// 	{
// 		// 解析出
// 		GetPrivateProfileString(g_psRegWTransSecName2, 
// 			sRegTemp, 
// 			pErrInfo, 
// 			sRegTemp1, 
// 			INI_FILE_LINE_MAXCHAR_NUM, 
// 			sIniFileName);
// 		
// 		if(strcmp(pErrInfo, sRegTemp1) == 0)
// 		{
// 			strcpy(sRegTemp1,"0");
// 		}	
// 		
// 		WritePrivateProfileString(g_psRegSectionName,sRegTemp,sRegTemp1,sIniFileName);
// 	}
#endif
	const char* pErrInfo = "Error: To Read Register failed";
    char sWTTRegAddr[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegTemp1[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
    DWORD dwParaSum = 0;
    DWORD i = 0;

	// 查询待转移的数据寄存器的地址
    strcpy(sWTTRegAddr, sRegAddr);
    strcat(sWTTRegAddr, "_WTW_str");
    //
	GetPrivateProfileString(g_psRegWTransSecName2, 
		sWTTRegAddr, 
		pErrInfo, 
		sRegTemp, 
		INI_FILE_LINE_MAXCHAR_NUM, 
		sIniFileName);

	if(strcmp(pErrInfo, sRegTemp) != 0)
	{
        // 解析待转移的数据寄存器（可能存在多个），放入了全局数组g_sParaSovle中
        dwParaSum = IniFileParseParamter(sRegTemp);
		
        // 根据解析出来的数据寄存器和用户定义在其中的数据，将数据写入相应的数据寄存器
        for(i = 0; i < dwParaSum; i++)
        {
            // 读出待转移到数据寄存器中的数据值
            GetPrivateProfileString(g_psRegWTransSecName2, 
                //sRegTemp, 
                g_sParaSovle[i],
                pErrInfo, 
                sRegTemp1, 
                INI_FILE_LINE_MAXCHAR_NUM, 
                sIniFileName);
            
            if(strcmp(pErrInfo, sRegTemp1) == 0)
            {
                return ERR_UNSUPPORT_OPERATION;
                //strcpy(sRegTemp1,"0");
            }	
            WritePrivateProfileString(g_psRegSectionName,
                                      g_sParaSovle[i],              //sRegTemp,
                                      sRegTemp1,sIniFileName);
        }
	}
	return ERR_SUCCESS;
}
/*****************************************************************
****
** 函数名:      IniFileOpIndirect_Read()
** 输  入:      需要解析的源头
** 输  出:      无
** 描  述:		以','为分割点解析参数
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-12-13
** 修    改:      
** 日    期:
*****************************************************************/
static DWORD IniFileIndirect_WR(const char* sRegAddr, const DWORD dwRegValue,
						 		  const char* sIniFileName)
{
	
	const char* pErrInfo = "Error: To Read Register failed";
	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sRegTemp1[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sTransControl[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sDataSave[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
	char sWillWriteData[INI_FILE_LINE_MAXCHAR_NUM] = {0};
	char sRegAddr_Indirect[INI_FILE_LINE_MAXCHAR_NUM] = {0};

	
	DWORD dwParaSum = 0;
	DWORD i = 0;
	
	DWORD dwChannelNo = 0;
	DWORD dwRegMask = 0;
	DWORD dwReadCtrl = 0;
	DWORD dwReadWriteCtrlBits = 0;
	BYTE  ucIsReadOrWrite = 0;

	strcpy(sRegAddr_Indirect, sRegAddr);
	strcat(sRegAddr_Indirect, "_INDIRECT_str");

	GetPrivateProfileString(g_psRegINDIRECT_WRSecName,
							sRegAddr_Indirect, 
							pErrInfo, 
							sRegTemp, 
							INI_FILE_LINE_MAXCHAR_NUM, 
							sIniFileName);
	
	if(strcmp(sRegTemp, pErrInfo) != 0)
	{
		// 解析sRegTemp中的值，获取间接控制寄存器的写入值
		// 根据传入的寄存器值和掩码进行计算得到该写入数据寄存器的值
		dwParaSum = IniFileParseParamter(sRegTemp);
		if(dwParaSum < 5)
		{
			return ERR_UNSUPPORT_OPERATION;
		}

		// 下一个要读取寄存器的值
		strcpy(sRegTemp1, g_sParaSovle[0]);
		strcat(sRegTemp1, "_INDIRECT_str");
		//
		strcpy(sTransControl, g_sParaSovle[4]);
		// 写控制寄存器
		WritePrivateProfileString(g_psRegSectionName,sRegAddr,sTransControl,sIniFileName);

		// 判断是读操作还是写操作
		dwReadWriteCtrlBits = CSS_IstHexStrToHex(g_sParaSovle[2]);
		dwReadCtrl = CSS_IstHexStrToHex(g_sParaSovle[3]);
		
		//1－表示读操作
		if( ((dwRegValue >> dwReadWriteCtrlBits)&dwReadCtrl) == dwReadCtrl)
		{
			ucIsReadOrWrite = 1;
		}
		else
		{
			ucIsReadOrWrite = 0;
		}
		

		
		if(ucIsReadOrWrite == 1) // 读操作
		{
			// 计算通道号，控制寄存器的值	
			dwRegMask      = CSS_IstHexStrToHex(g_sParaSovle[1]);
			dwChannelNo    = dwRegMask&dwRegValue;
			// 读取每个通道的寄存器值，然后解析
			GetPrivateProfileString(g_psRegINDIRECT_WRSecName, 
									sRegTemp1, 
									pErrInfo, 
									sRegTemp, 
									INI_FILE_LINE_MAXCHAR_NUM, 
									sIniFileName);
			// 构造数据寄存器的地址
			strcpy(sRegTemp1, g_sParaSovle[0]);
			// 构造数据寄存器中的内容
			dwParaSum = IniFileParseParamter(sRegTemp);
			//
			if(dwChannelNo >= dwParaSum)
			{
				strcpy(sRegTemp, "0");
			}				
			strcpy(sRegTemp, g_sParaSovle[dwChannelNo]);
			// 数据寄存器
			WritePrivateProfileString(g_psRegSectionName,sRegTemp1,sRegTemp,sIniFileName);	
		}
		else
		{
			GetPrivateProfileString(g_psRegSectionName, 
							        g_sParaSovle[0], 
							        pErrInfo, 
							        sWillWriteData, 
							        INI_FILE_LINE_MAXCHAR_NUM, 
							        sIniFileName);

			// 计算通道号，控制寄存器的值	
			dwRegMask      = CSS_IstHexStrToHex(g_sParaSovle[1]);
			dwChannelNo    = dwRegMask&dwRegValue;
			// 读取每个通道的寄存器值，然后解析
			GetPrivateProfileString(g_psRegINDIRECT_WRSecName, 
									sRegTemp1, 
									pErrInfo, 
									sRegTemp, 
									INI_FILE_LINE_MAXCHAR_NUM, 
									sIniFileName);
			
			//
			dwParaSum = IniFileParseParamter(sRegTemp);
			//
			if(dwChannelNo >= INI_FILE_LINE_MAXCHAR_NUM)
			{
				return ERR_INVALID_PARA;
			}				
			// 覆盖要写入通道处的数据
			strcpy(g_sParaSovle[dwChannelNo], sWillWriteData);

			// 组包成为以','分隔的字符串
			memset(sDataSave, 0, INI_FILE_LINE_MAXCHAR_NUM);
			for(i = 0; i < dwParaSum; i++)
			{
				strcat(sDataSave, g_sParaSovle[i]);
				strcat(sDataSave, ",");
			}
			sDataSave[strlen(sDataSave)-1] = 0;

			// 保存写入的数据			
			WritePrivateProfileString(g_psRegINDIRECT_WRSecName,sRegTemp1,sDataSave,sIniFileName);	
		}
		
	}
	else
	{
		return ERR_INVALID_PARA;
	}

	return ERR_SUCCESS;
}


/*****************************************************************
****
** 函数名:      IniFileConvToIniFileName()
** 输  入:      
** 输  出:      无
** 描  述:		读取指定地址的寄存器值
			    寄存器地址和寄存器内保存的值都是十六进制表示的，
				属性字段为"WR", "R", 用于寄存器的硬件特性，
				"WR"为可读可写， "R"-为只读
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-11-19 
** 修    改:      
** 日    期:
*****************************************************************/
static DWORD IniFileConvToIniFileName(const char* sIniFileRelPath, char* sIniFileName)
{
	DWORD i = 0;
//	DWORD j = 0;
	
	strcpy(sIniFileName, sIniFileRelPath);
	
	for( i = strlen(sIniFileName); i > 0; i--)
	{
		if(sIniFileName[i] == '.')
		{
			break;
		}
	}

    sIniFileName[i+1] = 'i';
	sIniFileName[i+2] = 'n';
	sIniFileName[i+3] = 'i';
	sIniFileName[i+4] = 0;
	
	return ERR_SUCCESS;
}
/*****************************************************************
****
** 函数名:      IniFileParseParamter()
** 输  入:      需要解析的源头
** 输  出:      无
** 描  述:		以','为分割点解析参数
** 全局变量:    
** 调用模块:   
** 作    者:    刘林	
** 日    期:	2005-12-13
** 修    改:      
** 日    期:
*****************************************************************/
static DWORD IniFileParseParamter(const char* sLineText)
{
	char sDesText[INI_FILE_LINE_MAXCHAR_NUM];
	DWORD cnt = 0;
	DWORD j = 0;
	DWORD i = 0;

	memset(g_sParaSovle, 0, INI_FILE_LINE_MAXCHAR_NUM*INI_FILE_LINE_MAXCHAR_NUM);

	CSS_IstFormat(sDesText, sLineText);

	if(strlen(sDesText) == 0)
	{
		return 0;
	}

	for(i = 0; i < strlen(sDesText); i++)
	{
		if(sDesText[i] != ',')
		{
			g_sParaSovle[cnt][j] = sDesText[i];
			j++;
		}
		else
		{
			cnt++;
			j = 0;
		}
	}
	return cnt+1;
}

/**********************************************************************
* 函数名称： CSS_IstHexStrToHex
* 功能描述： 将一个十六进制字符串到数值的转换
* 输入参数： 待转换的字符串
* 输出参数： 无 
* 返 回 值： 转换后的数值
* 其它说明： 
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2004.11.05       V1.0	      刘林 	        创建
***********************************************************************/
DWORD CSS_IstHexStrToHex(const char *pBuffer)
{
    DWORD  dwOutHexValue = 0;
	DWORD  j = 0;
	char* pTempBuffer = (char*)pBuffer;

	// 传入的十六进制字符串前面是以"0x"开始的，则先去掉
	if((pBuffer[0] == '0') && (tolower(pBuffer[1]) == 'x'))
	{
		pTempBuffer += 2;
	}
    DWORD  len = strlen(pTempBuffer);
	for(j = 0;j < len; j++)
	{	
		if((*pTempBuffer >='0')&&(*pTempBuffer <='9')) 
		{
			dwOutHexValue = (dwOutHexValue<<4) + *pTempBuffer - '0';
		}
		else if((*pTempBuffer >='A')&&(*pTempBuffer <='F')) 
		{
			dwOutHexValue = (dwOutHexValue<<4) + *pTempBuffer - 'A' + 10;
		}
		else if((*pTempBuffer >='a')&&(*pTempBuffer <='f'))
		{
			dwOutHexValue = (dwOutHexValue<<4) + *pTempBuffer - 'a' + 10;
		}
		else
		{
			break;
		}
		pTempBuffer++;
	}
    return dwOutHexValue;
}

/*******************************************************************
** 函数名:      IstFormat()
** 输  入:      pDesStr-为格式化后的字符串指针  pSrcStr-为格式化前的
                字符串指针
** 输  出:      无
** 描  述:      将命令行格式化成为标准的命令行，去掉命令行最前面的空格，
                去掉参数之间多余一个空格
** 全局变量:	无
** 调用模块:	无
** 作    者:	刘林
** 日    期:	2003.3.27
** 修 改 人:    刘林
** 修    改:    
** 日    期:    2005-02-20 12:10
**
** 修 改 人:    袁海
** 修    改:    去掉命令行最后面的空格和回车符号，解决参数错误的问题
** 日    期:    2006-09-14
**********************************************************************/ 
HERROR CSS_IstFormat(char *pDesStr, const char *pSrcStr)
{
	if((pDesStr == NULL) || (pSrcStr == NULL))
	{
	    return ERR_MEM_POINTER_NULL;
	}
	
	/* 去掉命令行前面的空格 */
    while (*pSrcStr == 0x20)
	{
		pSrcStr++;
	}
	if((*pSrcStr) != 0)
	{
		do
		{
			/* 如果连续两个空格则去掉一个多余的空格 */
			while( ((*pSrcStr) == 0x20 && (*(pSrcStr+1)) == 0x20)
				|| ((*pSrcStr) == 0x20 && (*(pSrcStr+1)) == 0x0A)
				|| ((*pSrcStr) == 0x0A && (*(pSrcStr+1)) == 0x0A)
				|| ((*pSrcStr) == 0x0A && (*(pSrcStr+1)) == 0x20)
				) 
			{  
				pSrcStr++;
			}

			if((*pSrcStr) == ' ' && (*(pSrcStr+1)) == 0)
			{
				pSrcStr++;
			}
						
			/* 全部转换成小写 */
			*pDesStr = *pSrcStr;
			
			/* 如果是结束符则直接退出循环*/
			if((*pSrcStr) == 0)
			{
				break;
			}
			
			if(*pDesStr == 0x0A)
			{
				*pDesStr = 0x20;
			}
			*pDesStr = tolower(*pDesStr);
			pDesStr++;
		}while((*(pSrcStr++)) != 0);
	}
	return ERR_SUCCESS;
}
/*******************************************************************
** 函数名:      istShellpara()
** 输  入:      s2-为格式化后的字符串指针  s1-为格式化前的字符串指针
** 输  出:      无
** 描  述:      从命令行中解析出参数，存放在二维数组g_istCmdPara中
** 全局变量:	无
** 调用模块:	无
** 作    者:	刘林
** 日    期:	2003.3.27
** 修    改:
** 日    期:
**********************************************************************/ 
HERROR IstShellpara(char* buf,char* ap)
{
	static WORD i = 0;
	WORD j = 0;
	
	buf += i;
	if( (*buf == '\0') || (strlen(buf) == 0) )
	{
		i = 0;
		return 0;
	}
	
	while(1)
	{
		if( (*buf == ' ') || (*buf == '\0'))
			break;

		ap[j++] = (*buf);
		buf++;
		i++;
	}
	i++;
	ap[j] = '\0';
	return 1;
}
#endif