// RxADO.cpp: implementation of the RxADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RxADO.h"
#include "ExternDLLHeader.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
_ConnectionPtr cnn;
RxADO::RxADO()
{

}

RxADO::~RxADO()
{

}

bool RxADO::SetConnection(CString LinkString)
{
	::CoInitialize(NULL);
	cnn=NULL;
	cnn.CreateInstance(__uuidof(Connection));
	cnn->ConnectionString=(_bstr_t)LinkString;
	try{
		cnn->Open(L"",L"",L"",adCmdUnspecified);
	}
	catch(_com_error& e)
	{
		ErrorsPtr pErrors=cnn->GetErrors();
		if (pErrors->GetCount()==0)
		{	
			CString ErrMsg=e.ErrorMessage();
			MessageBox(NULL,"发生错误:\n\n"+ErrMsg,"系统提示",MB_OK|MB_ICONEXCLAMATION);	
			return false;
		}
		else
		{
			for (int i=0;i<pErrors->GetCount();i++)
			{
				_bstr_t desc=pErrors->GetItem((long)i)->GetDescription();
				MessageBox(NULL,"发生错误:\n\n"+desc,"系统提示",MB_OK|MB_ICONEXCLAMATION);
				return false;
			}
		}	
	}
	return true;
}
/*
 当nStale=1时 字符字段返回字符字段
 当nStale=2时 数值字段返回字符字段
 当nStale=3时 日期字段返回字符字段
*/
CString RxADO::FieldToOtherField(CString cDataBaseName, CString cFieldName, CString cValue, CString cReturnField, int nStale=1)
{
	CString sSQL,sRTValue;
	_RecordsetPtr Fieldrst;
	Fieldrst.CreateInstance(__uuidof(Recordset));
	Fieldrst.CreateInstance(__uuidof(Recordset));
	if(nStale==1)	//字符字段返回字符字段
			sSQL.Format("SELECT * FROM %s WHERE %s='%s'",cDataBaseName,cFieldName,cValue);
	if(nStale==2)//数值字段返回字符字段
			sSQL.Format("SELECT * FROM %s WHERE %s=%s",cDataBaseName,cFieldName,cValue);
	if(nStale==3)//日期字段返回字符字段
			sSQL.Format("SELECT * FROM %s WHERE %s=#%s#",cDataBaseName,cFieldName,cValue);
	try{
		Fieldrst=cnn->Execute((_bstr_t)sSQL,NULL,adCmdText);
	}
	catch(_com_error&e)
	{
		GetADOErrors(e);
		return "";
	}
	if(GetRecordCount(Fieldrst)<=0)
		return "";
	_variant_t vValue=Fieldrst->GetCollect((_bstr_t)cReturnField);
	if(vValue.vt==VT_EMPTY)
		return "";
	sRTValue=(char*)(_bstr_t)vValue;
	sRTValue.TrimRight();
	sRTValue.TrimLeft();
	return sRTValue;
}

void RxADO::GetADOErrors(_com_error eErrors)
{
	ErrorsPtr pErrors=cnn->GetErrors();
	if (pErrors->GetCount()==0)	
		MessageBox(NULL,eErrors.ErrorMessage(),"错  误",MB_OK|MB_ICONEXCLAMATION);	
	else
	{
		for (int i=0;i<pErrors->GetCount();i++)
		{
			_bstr_t desc=pErrors->GetItem((long)i)->GetDescription();
			MessageBox(NULL,desc,"错  误",MB_OK|MB_ICONEXCLAMATION);
		}
	}
}

int RxADO::GetRecordCount(_RecordsetPtr pRst)
{
	int nCount=0;
	try{
		pRst->MoveFirst();
	}
	catch(...)
	{
		return 0;
	}
	if(pRst->_EOF)
		return 0;
	while (!pRst->_EOF)
	{
		pRst->MoveNext();
		nCount=nCount+1;	
	}
	pRst->MoveFirst();
	return nCount;
}

/*
	当nStyle=1时 生成数字编号
	当nStyle=2时 生成流水帐号
	当nStyle=3时 生成日期时间编号
*/
CString RxADO::AutoNumber(CString sTable, CString sFieldName, CString sCode, int nStyle)
{
 	_RecordsetPtr AutoNumberrst;
 	CString sTempNewNumber,sNewNumber,sSQL,sMaxNumber,sOldNumber;
 	AutoNumberrst.CreateInstance(__uuidof(Recordset));
 	sSQL.Format("SELECT MAX(%s) as 最大编号 FROM %s",sFieldName,sTable);
	try{
	AutoNumberrst=cnn->Execute((_bstr_t)sSQL,NULL,adCmdText);		
	}
	catch(_com_error & e)
	{
		GetADOErrors(e);
	}
	if(nStyle==1) //数字编号
 	{
 		if(GetRecordCount(AutoNumberrst)<1)
			sNewNumber.Format("1");
		else
	{
		AutoNumberrst->MoveFirst();
		_variant_t vtext;
		vtext=AutoNumberrst->GetCollect("最大编号");
		if(vtext.vt==VT_EMPTY||vtext.vt==VT_NULL)
		{
			sNewNumber.Format("1");
			goto end;
		}
		sMaxNumber=(char*)(_bstr_t)AutoNumberrst->GetCollect("最大编号");
			sNewNumber.Format("%d",atoi(sMaxNumber)+1);
		}
	}
	if(nStyle==2)//流水帐号
	{
		if(GetRecordCount(AutoNumberrst)<1)
			sNewNumber.Format("%s00000001",sCode);
		else
 		{
			AutoNumberrst->MoveFirst();
			_variant_t _bOldNumber=AutoNumberrst->GetCollect("最大编号");
			if(_bOldNumber.vt==VT_NULL || _bOldNumber.vt==VT_EMPTY)
			{
				sNewNumber.Format("%s00000001",sCode);
				goto end;
			}
			sOldNumber=(char*)(_bstr_t) _bOldNumber;
			sMaxNumber=sOldNumber.Mid(3); 
			sTempNewNumber.Format("%d",atoi(sMaxNumber)+1);
			sNewNumber.Format("%s%s",sCode,Padl(sTempNewNumber,8,"0",1));
		}

	}
	if(nStyle==3)//日期时间编号
	{
		int nYear,nDay,nMonth;
		CString sYear,sDay,sMonth;
		CTime tTime;
 		tTime=tTime.GetCurrentTime(); 
		nYear=tTime.GetYear();
		nDay=tTime.GetDay();
		nMonth=tTime.GetMonth();
		sYear.Format("%d",nYear);
		sDay.Format("%d",nDay);
		sMonth.Format("%d",nMonth);
 		
		sYear=sYear.Mid(2);
		sDay=Padl(sDay,2,"0",1);
		sMonth=Padl(sMonth,2,"0",1);
 		if(GetRecordCount(AutoNumberrst)<1)
			sNewNumber.Format("%s%s%s%s-000001  ",sCode,sYear,sMonth,sDay);
		else
		{
			_variant_t bh=AutoNumberrst->GetCollect("最大编号");
			if(bh.vt==VT_EMPTY||bh.vt==VT_NULL)
			{
				sNewNumber.Format("%s%s%s%s-000001  ",sCode,sYear,sMonth,sDay);
				goto end;
			}
			CString sOldDate,sNewDate;
			AutoNumberrst->MoveFirst();
 			sOldNumber=(char*)(_bstr_t)AutoNumberrst->GetCollect("最大编号");
 			sOldDate=sOldNumber.Mid(2,6);
			sNewDate.Format("%s%s%s",sYear,sMonth,sDay);
			if(sOldDate==sNewDate)
			{
				sMaxNumber=sOldNumber.Mid(9);
				sTempNewNumber.Format("%d",atoi(sMaxNumber)+1);
				sNewNumber.Format("%s%s%s%s-%s",sCode,sYear,sMonth,sDay,Padl(sTempNewNumber,6,"0",1));
 			}
 			else
				sNewNumber.Format("%s%s%s%s-000001  ",sCode,sYear,sMonth,sDay);		
		}
 	}
end: 	return sNewNumber;
}




_ConnectionPtr RxADO::GetConnection()
{
	return cnn;
}

void RxADO::Close()
{
	cnn->Close();
}
