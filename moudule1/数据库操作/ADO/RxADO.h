// RxADO.h: interface for the RxADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RXADO_H__4B731009_E70A_4FE6_A9DF_7F8E87010D3C__INCLUDED_)
#define AFX_RXADO_H__4B731009_E70A_4FE6_A9DF_7F8E87010D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RxRecordset.h"
class RxADO  
{
public:
	void Close();
	_ConnectionPtr cnn;
	_ConnectionPtr GetConnection();
	RxRecordset record;
	CString AutoNumber(CString sTable, CString sFieldName, CString sCode, int nStyle=1);
	int GetRecordCount(_RecordsetPtr pRst);
	void GetADOErrors(_com_error eErrors);
	CString FieldToOtherField(CString cDataBaseName, CString cFieldName, CString cValue, CString cReturnField, int nStale);
	bool SetConnection(CString LinkString);
	RxADO();
	virtual ~RxADO();


};

#endif // !defined(AFX_RXADO_H__4B731009_E70A_4FE6_A9DF_7F8E87010D3C__INCLUDED_)
