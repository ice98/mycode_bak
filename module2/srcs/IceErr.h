#ifndef _ICE_ERR_H
#define _ICE_ERR_H



#include "IceBaseType.h"

#define MAX_FILEPATH_LENG (255)

typedef map<string, unsigned long> MapError;


typedef struct  
{
	DWORD m_ulLineNo;
	DWORD m_ulErrCode;
	string m_strErrFileName;
	string m_strErrMsg;
}T_ErrInfo;

typedef stack<T_ErrInfo> StackForError;
extern StackForError g_objErrStack;

HERROR PrintError(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR strMsgInfo = NULL, DWORD dwErrCode = 0);
HERROR RecordError(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR strMsgInfo = NULL, DWORD dwErrCode = 0);
HERROR RecordErrorWithPrint(LPCSTR pszFilePathName, DWORD dwLineNo, LPCSTR strMsgInfo = NULL, DWORD dwErrCode = 0);
HERROR GetNameFromPath(const CHAR *pszFilePathName, CHAR *pszFileName);

#endif
