#include "IceErr.h"

StackForError g_objErrStack;

HERROR PrintError(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR pszMsgInfo, DWORD dwErrCode)
{
	CHAR szFileName[MAX_FILEPATH_LENG] = {0};
	CHAR szErrInfo[255] = {0};
	GetNameFromPath(pszFilePathName, szFileName);
	sprintf(szErrInfo,"Error %d: %s,line %d\n ->%s\n",dwErrCode, szFileName, dwLineNo, pszMsgInfo);

#ifndef _AFXDLL
	cout << szErrInfo << endl;
#else
	AfxMessageBox(szErrInfo);
#endif

	return SUCCESS;
}

HERROR RecordError(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR pszMsgInfo, DWORD dwErrCode)
{
	CHAR szFileName[MAX_FILEPATH_LENG] = {0};
	CHAR szErrInfo[255] = {0};
	GetNameFromPath(pszFilePathName, szFileName);
	T_ErrInfo t_errInfo;

	t_errInfo.m_strErrFileName.assign(szFileName);
	t_errInfo.m_strErrMsg.assign(pszMsgInfo);
	t_errInfo.m_ulErrCode = dwErrCode;
	t_errInfo.m_ulLineNo = dwLineNo;

	g_objErrStack.push(t_errInfo);

	return SUCCESS;
}

HERROR RecordErrorWithPrint(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR pszMsgInfo, DWORD dwErrCode)
{
	return SUCCESS;
}

HERROR GetNameFromPath(LPCSTR pszFilePathName, CHAR *pszFileName)
{
	string strTemp(pszFilePathName);
	INT pos = strTemp.rfind('\\');
	strTemp = strTemp.substr(pos+1);
	CHAR *pszTemp = (char*) strTemp.c_str();
	memcpy(pszFileName, pszTemp, strlen(pszTemp));

	return SUCCESS;

}