/*****************************************************************
****
** �ļ���:  Inifile.c
** ��Ȩ:    ��������ͨѶ�ɷ����޹�˾����
** ������:	����
** ����:    2005-11-18
** �޸���:  
** ����:    
** ����:	
** �汾:    V1.0
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
//  �ڲ���������
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
//  �ڲ�ȫ�ֱ����Ķ���
*****************************************************************/
/* �����ļ��� */
/*static char g_sNameFile[INI_FILE_LINE_MAXCHAR_NUM];*/
/* ����Ĵ�����SECTION���� */
static const char* g_psRegSectionName = "ICChip_SimuRegister";
static char  g_sParaSovle[INI_FILE_LINE_MAXCHAR_NUM][INI_FILE_LINE_MAXCHAR_NUM];



/* ����Ĵ������ԵĶ�����*/
static const char* g_psRegProSecName = "REG_PROPERTY";
/* ����Ĵ��������� */
static const char* g_psRegProKey_ONLY_READ   = "ONLY_READ";
static const char* g_psRegProKey_READ_CLEAR  = "READ_CLEAR";
static const char* g_psRegProKey_WRITE_CLEAR = "WRITE_CLEAR";
// дת���ͼĴ�������
static const char* g_psRegProKey_WRITE_TRANS1 = "WRITE_TRANS_WAY1";
static const char* g_psRegProKey_WRITE_TRANS2 = "WRITE_TRANS_WAY2";

// дת���ͼĴ�������д�벻ͬ��ֵ����Ĵ�����ֵ�Զ��仯
static const char* g_psRegProKey_WRITE_TRANS3 = "WRITE_TRANS_WAY3";

// ��Ӷ�дȡоƬ��ֵ��1�����ƼĴ�����1�����ݼĴ�����ģʽ��
static const char* g_psRegProKey_INDIRECT_WR = "INDIRECT_WR";



// �󶨼Ĵ����Ķ��壬��д����ʱ�����ԵȺ����ҵļĴ���ͬʱ��������ȹ�����
static const char* g_psRegBindSecName = "REG_EQU_BIND";

// дת�ƶ�����
// ���ַ�ʽ��ʾֱ��ת�Ƶ�д������Ĵ�����
static const char* g_psRegWTransSecName1 = "REG_WRITE_TRANS_WAY1";
// ���ַ�ʽ��ʾֱ��ת�Ƶ������ļĴ�����
static const char* g_psRegWTransSecName2 = "REG_WRITE_TRANS_WAY2";
// ��Ӷ�ȡ�Ķ�����
static const char* g_psRegINDIRECT_WRSecName = "REG_INDIRECT_WR";



/*****************************************************************
****
** ������:      IniFileReadReg()
** ��  ��:      ��ȡ�ļĴ�����ַ
** ��  ��:      ��
** ��  ��:		��ȡָ����ַ�ļĴ���ֵ
			    �Ĵ�����ַ�ͼĴ����ڱ����ֵ����ʮ�����Ʊ�ʾ�ģ�
				�����ֶ�Ϊ"WR", "R", ���ڼĴ�����Ӳ�����ԣ�
				"WR"Ϊ�ɶ���д�� "R"-Ϊֻ��
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-11-18 
** ��    ��:      
** ��    ��:
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
	
	/*���Ĵ�����ַת��Ϊ�ַ���*/
	sprintf(sRegAddr,"%x", dwRegAddr);

	IniFileConvToIniFileName(sIniFileRelPath,sIniFileName);

	/* ���Ĵ��� */
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

	/* ���Ĵ��������ԣ��������Ծ�����һ������ */
	GetPrivateProfileString(g_psRegProSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegProt, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);

	/* ��ѯ���˼Ĵ����������� */
	if(strcmp(pErrInfo, sRegProt) != 0)
	{
		/* ����Ƕ������㣬��д�㵽�Ĵ�����*/
		if(strstr(sRegProt, g_psRegProKey_READ_CLEAR) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegAddr,"0",sIniFileName);
		}
		
	}

	return dwRegValue;
}

/*****************************************************************
****
** ������:      IniFileWriteReg()
** ��  ��:      
** ��  ��:      ��
** ��  ��:		��ȡָ����ַ�ļĴ���ֵ
			    �Ĵ�����ַ�ͼĴ����ڱ����ֵ����ʮ�����Ʊ�ʾ�ģ�
				�����ֶ�Ϊ"WR", "R", ���ڼĴ�����Ӳ�����ԣ�
				"WR"Ϊ�ɶ���д�� "R"-Ϊֻ��
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-11-19 
** ��    ��:      
** ��    ��:
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
	

	// ���Ĵ�����ַת��Ϊ�ַ���
	sprintf(sRegAddr,"%x",  dwRegAddr);
	sprintf(sRegValue,"%x", dwRegValue);

	IniFileConvToIniFileName(sIniFileRelPath,sIniFileName);
	
	for(i = 0; i < strlen(sRegValue); i++)
	{
		sRegValue[i] = toupper(sRegValue[i]);
	}

	strcpy(sRegAddrProt, sRegAddr);
	strcat(sRegAddrProt,"str");
	/* ���Ĵ��������ԣ��������Ծ�����һ������ */
	GetPrivateProfileString(g_psRegProSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegProt, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);

	/* ���ݹ����Ĵ���������д���ݵ�������ļĴ����� */
	GetPrivateProfileString(g_psRegBindSecName, 
                            sRegAddrProt, 
                            pErrInfo, 
                            sRegBind, 
                            INI_FILE_LINE_MAXCHAR_NUM, 
                            sIniFileName);
	/* ��ѯ���˼Ĵ����������� */
	if(strcmp(pErrInfo, sRegProt) != 0)
	{
		/* �����д�����㣬��д�㵽�Ĵ�����*/
		if(strstr(sRegProt, g_psRegProKey_READ_CLEAR) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegAddr,"0",sIniFileName);
			/* ������ڰ󶨼Ĵ���������ִ��*/
			if(strcmp(pErrInfo, sRegBind) != 0)
			{
				WritePrivateProfileString(g_psRegSectionName,sRegBind,"0",sIniFileName);
			}
		}
		else if(strstr(sRegProt, g_psRegProKey_WRITE_TRANS1) != 0) // дת���Ͳ�����ʽ1
		{
			IniFileWrite_Trans_way1(sRegAddr, dwRegValue, sIniFileName);
		}		
		else if(strstr(sRegProt, g_psRegProKey_WRITE_TRANS2) != 0) // дת���Ͳ�����ʽ2
		{
			IniFileWrite_Trans_way2(sRegAddr, dwRegValue, sIniFileName);
		}
		else if(strstr(sRegProt, g_psRegProKey_INDIRECT_WR) != 0) // ��Ӷ�ȡ
		{

			IniFileIndirect_WR(sRegAddr, dwRegValue, sIniFileName);
		}
		else if(strstr(sRegProt, g_psRegProKey_ONLY_READ) != 0) // ֻ���Ĵ�������д��
		{
			;
		}
		else
		{
			;
		}
	}
	else // �ɶ���д����ͨ�Ĵ���
	{
		WritePrivateProfileString(g_psRegSectionName, 
								sRegAddr, 
								sRegValue, 
								sIniFileName); 
		/* ������ڰ󶨼Ĵ���������ִ��*/
		if(strcmp(pErrInfo, sRegBind) != 0)
		{
			WritePrivateProfileString(g_psRegSectionName,sRegBind,sRegValue,sIniFileName);
		}
	}


		
	return ERR_SUCCESS;
}
/*****************************************************************
****
** ������:      IniFileWrite_Trans_way1()
** ��  ��:      ��Ҫ������Դͷ
** ��  ��:      ��
** ��  ��:		
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-12-13
** ��    ��:      
** ��    ��:
*****************************************************************/
static DWORD IniFileWrite_Trans_way1(const char* sRegAddr, const DWORD dwRegValue,
						 		  const char* sIniFileName)
{
	const char* pErrInfo = "Error: To Read Register failed";
	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
		
	// ��ѯת���ֶε���ز���
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
** ������:      IniFileWrite_Trans_way2()
** ��  ��:      ��Ҫ������Դͷ
** ��  ��:      ��
** ��  ��:		��','Ϊ�ָ���������
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-12-13
** ��    ��:      
** ��    ��:
*****************************************************************/
static DWORD IniFileWrite_Trans_way2(const char* sRegAddr, const DWORD dwRegValue,
									 const char* sIniFileName)
									 
{
#if 0
// 	const char* pErrInfo = "Error: To Read Register failed";
// 	char sRegTemp[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
// 	char sRegTemp1[INI_FILE_LINE_MAXCHAR_NUM]  = {0};
// 
// 	// ��ѯת���ֶε���ز���
// 	GetPrivateProfileString(g_psRegWTransSecName2, 
// 		sRegAddr, 
// 		pErrInfo, 
// 		sRegTemp, 
// 		INI_FILE_LINE_MAXCHAR_NUM, 
// 		sIniFileName);
// 	
// 	if(strcmp(pErrInfo, sRegTemp) != 0)
// 	{
// 		// ������
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

	// ��ѯ��ת�Ƶ����ݼĴ����ĵ�ַ
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
        // ������ת�Ƶ����ݼĴ��������ܴ��ڶ������������ȫ������g_sParaSovle��
        dwParaSum = IniFileParseParamter(sRegTemp);
		
        // ���ݽ������������ݼĴ������û����������е����ݣ�������д����Ӧ�����ݼĴ���
        for(i = 0; i < dwParaSum; i++)
        {
            // ������ת�Ƶ����ݼĴ����е�����ֵ
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
** ������:      IniFileOpIndirect_Read()
** ��  ��:      ��Ҫ������Դͷ
** ��  ��:      ��
** ��  ��:		��','Ϊ�ָ���������
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-12-13
** ��    ��:      
** ��    ��:
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
		// ����sRegTemp�е�ֵ����ȡ��ӿ��ƼĴ�����д��ֵ
		// ���ݴ���ļĴ���ֵ��������м���õ���д�����ݼĴ�����ֵ
		dwParaSum = IniFileParseParamter(sRegTemp);
		if(dwParaSum < 5)
		{
			return ERR_UNSUPPORT_OPERATION;
		}

		// ��һ��Ҫ��ȡ�Ĵ�����ֵ
		strcpy(sRegTemp1, g_sParaSovle[0]);
		strcat(sRegTemp1, "_INDIRECT_str");
		//
		strcpy(sTransControl, g_sParaSovle[4]);
		// д���ƼĴ���
		WritePrivateProfileString(g_psRegSectionName,sRegAddr,sTransControl,sIniFileName);

		// �ж��Ƕ���������д����
		dwReadWriteCtrlBits = CSS_IstHexStrToHex(g_sParaSovle[2]);
		dwReadCtrl = CSS_IstHexStrToHex(g_sParaSovle[3]);
		
		//1����ʾ������
		if( ((dwRegValue >> dwReadWriteCtrlBits)&dwReadCtrl) == dwReadCtrl)
		{
			ucIsReadOrWrite = 1;
		}
		else
		{
			ucIsReadOrWrite = 0;
		}
		

		
		if(ucIsReadOrWrite == 1) // ������
		{
			// ����ͨ���ţ����ƼĴ�����ֵ	
			dwRegMask      = CSS_IstHexStrToHex(g_sParaSovle[1]);
			dwChannelNo    = dwRegMask&dwRegValue;
			// ��ȡÿ��ͨ���ļĴ���ֵ��Ȼ�����
			GetPrivateProfileString(g_psRegINDIRECT_WRSecName, 
									sRegTemp1, 
									pErrInfo, 
									sRegTemp, 
									INI_FILE_LINE_MAXCHAR_NUM, 
									sIniFileName);
			// �������ݼĴ����ĵ�ַ
			strcpy(sRegTemp1, g_sParaSovle[0]);
			// �������ݼĴ����е�����
			dwParaSum = IniFileParseParamter(sRegTemp);
			//
			if(dwChannelNo >= dwParaSum)
			{
				strcpy(sRegTemp, "0");
			}				
			strcpy(sRegTemp, g_sParaSovle[dwChannelNo]);
			// ���ݼĴ���
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

			// ����ͨ���ţ����ƼĴ�����ֵ	
			dwRegMask      = CSS_IstHexStrToHex(g_sParaSovle[1]);
			dwChannelNo    = dwRegMask&dwRegValue;
			// ��ȡÿ��ͨ���ļĴ���ֵ��Ȼ�����
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
			// ����Ҫд��ͨ����������
			strcpy(g_sParaSovle[dwChannelNo], sWillWriteData);

			// �����Ϊ��','�ָ����ַ���
			memset(sDataSave, 0, INI_FILE_LINE_MAXCHAR_NUM);
			for(i = 0; i < dwParaSum; i++)
			{
				strcat(sDataSave, g_sParaSovle[i]);
				strcat(sDataSave, ",");
			}
			sDataSave[strlen(sDataSave)-1] = 0;

			// ����д�������			
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
** ������:      IniFileConvToIniFileName()
** ��  ��:      
** ��  ��:      ��
** ��  ��:		��ȡָ����ַ�ļĴ���ֵ
			    �Ĵ�����ַ�ͼĴ����ڱ����ֵ����ʮ�����Ʊ�ʾ�ģ�
				�����ֶ�Ϊ"WR", "R", ���ڼĴ�����Ӳ�����ԣ�
				"WR"Ϊ�ɶ���д�� "R"-Ϊֻ��
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-11-19 
** ��    ��:      
** ��    ��:
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
** ������:      IniFileParseParamter()
** ��  ��:      ��Ҫ������Դͷ
** ��  ��:      ��
** ��  ��:		��','Ϊ�ָ���������
** ȫ�ֱ���:    
** ����ģ��:   
** ��    ��:    ����	
** ��    ��:	2005-12-13
** ��    ��:      
** ��    ��:
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
* �������ƣ� CSS_IstHexStrToHex
* ���������� ��һ��ʮ�������ַ�������ֵ��ת��
* ��������� ��ת�����ַ���
* ��������� �� 
* �� �� ֵ�� ת�������ֵ
* ����˵���� 
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2004.11.05       V1.0	      ���� 	        ����
***********************************************************************/
DWORD CSS_IstHexStrToHex(const char *pBuffer)
{
    DWORD  dwOutHexValue = 0;
	DWORD  j = 0;
	char* pTempBuffer = (char*)pBuffer;

	// �����ʮ�������ַ���ǰ������"0x"��ʼ�ģ�����ȥ��
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
** ������:      IstFormat()
** ��  ��:      pDesStr-Ϊ��ʽ������ַ���ָ��  pSrcStr-Ϊ��ʽ��ǰ��
                �ַ���ָ��
** ��  ��:      ��
** ��  ��:      �������и�ʽ����Ϊ��׼�������У�ȥ����������ǰ��Ŀո�
                ȥ������֮�����һ���ո�
** ȫ�ֱ���:	��
** ����ģ��:	��
** ��    ��:	����
** ��    ��:	2003.3.27
** �� �� ��:    ����
** ��    ��:    
** ��    ��:    2005-02-20 12:10
**
** �� �� ��:    Ԭ��
** ��    ��:    ȥ�������������Ŀո�ͻس����ţ�����������������
** ��    ��:    2006-09-14
**********************************************************************/ 
HERROR CSS_IstFormat(char *pDesStr, const char *pSrcStr)
{
	if((pDesStr == NULL) || (pSrcStr == NULL))
	{
	    return ERR_MEM_POINTER_NULL;
	}
	
	/* ȥ��������ǰ��Ŀո� */
    while (*pSrcStr == 0x20)
	{
		pSrcStr++;
	}
	if((*pSrcStr) != 0)
	{
		do
		{
			/* ������������ո���ȥ��һ������Ŀո� */
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
						
			/* ȫ��ת����Сд */
			*pDesStr = *pSrcStr;
			
			/* ����ǽ�������ֱ���˳�ѭ��*/
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
** ������:      istShellpara()
** ��  ��:      s2-Ϊ��ʽ������ַ���ָ��  s1-Ϊ��ʽ��ǰ���ַ���ָ��
** ��  ��:      ��
** ��  ��:      ���������н���������������ڶ�ά����g_istCmdPara��
** ȫ�ֱ���:	��
** ����ģ��:	��
** ��    ��:	����
** ��    ��:	2003.3.27
** ��    ��:
** ��    ��:
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