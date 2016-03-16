// RxRecordset.cpp: implementation of the RxRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "商品销售管理系统.h"
#include "RxRecordset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RxRecordset::RxRecordset()
{

}

RxRecordset::~RxRecordset()
{

}

CString RxRecordset::GetFieldValue(CString Field)
{
	CString sValue;
	_variant_t value;
	value=rst->GetCollect((_bstr_t)Field);
	if(value.vt==VT_EMPTY ||value.vt==VT_NULL)
		sValue="";
	else
	{
		sValue=(char*)(_bstr_t)value;
		sValue.TrimRight();
		sValue.TrimLeft();
	}
	return sValue;
}



bool RxRecordset::Open(CString sCommandText, UINT adCmd)
{
	if(adCmd==adCmdTable)
		sRecordset=sCommandText;
	else
		sRecordset="("+sCommandText+") DERIVEDTBL";
	try{
	 rst=cnn->Execute((_bstr_t)sCommandText,NULL,adCmd);
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

int RxRecordset::GetRecordCount()
{
	int nCount=0;
	try{
		rst->MoveFirst();
	}
	catch(...)
	{
		return 0;
	}
	if(rst->_EOF)
		return 0;
	while (!rst->_EOF)
	{
		rst->MoveNext();
		nCount=nCount+1;	
	}
	rst->MoveFirst();
	return nCount;
}

bool RxRecordset::MoveFirst()
{
	try{
	rst->MoveFirst();
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

bool RxRecordset::MoveNext()
{
	try{
	rst->MoveNext();
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

bool RxRecordset::MoveLast()
{
	try{
	rst->MoveLast();
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

bool RxRecordset::MovePrevious()
{
	try{
	rst->MovePrevious();
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

bool RxRecordset::Move(int nRecordNumber)
{
	try{
	rst->MoveFirst();
	rst->Move(nRecordNumber);
	}
	catch(_com_error&e)
	{
		this->GetErrors(e);
		return false;
	}
	return true;
}

int RxRecordset::GetFieldCount()
{
	int count;
	try{
		count=rst->GetFields()->GetCount();
	}
	catch(...)
	{
		return -1;
	}
	return count;
}

_RecordsetPtr RxRecordset::GetRecordset()
{
	return rst;
}

void RxRecordset::GetErrors(_com_error eErrors)
{
	CString string;
	CFile file;
	
	file.Open("Error.Rxe",CFile::modeWrite|CFile::modeNoTruncate);
	ErrorsPtr pErrors=cnn->GetErrors();
	if (pErrors->GetCount()==0)	
	{
		string=(char*)(_bstr_t)eErrors.ErrorMessage();
		file.Write(string+"\r\n",string.GetLength()+1);
		//::AfxMessageBox(string);
	}
	else
	{
		for (int i=0;i<pErrors->GetCount();i++)
		{
			_bstr_t desc=pErrors->GetItem((long)i)->GetDescription();
			string=(char*)desc;
			file.Write(string+"\r\n",string.GetLength()+1);
			//::AfxMessageBox(string);
		}
	}
	file.Close();
}

void RxRecordset::Close()
{
	rst->Close();
}

CString RxRecordset::GetFieldName(int nNumber)
{
	CString sName;
	_variant_t vName; 
	try{
		vName=rst->GetFields()->GetItem((long)nNumber)->GetName();
	}
	catch(_com_error&e)
	{
		GetErrors(e);
		return "";
	}
	if(vName.vt==VT_EMPTY)
		return "";
	sName=(char*)(_bstr_t)vName;
	sName.TrimLeft();
	sName.TrimRight();
	return sName;
}

CString RxRecordset::GetSQL()
{
	return sRecordset;
}

CString RxRecordset::GetFieldType(int nNumber)
{	
	ADODB::DataTypeEnum type;
	CString sType;
	try{
		rst->GetFields()->GetItem((long)nNumber)->get_Type(&type);
	}
	catch(_com_error&e)
	{
		GetErrors(e);
		return adError;
	}
	ADODB::DataTypeEnum tp[]={adBigInt,adBinary,adBoolean,adCurrency,adDecimal,adDouble,adInteger,adLongVarBinary,adNumeric,adSingle,adSmallInt,adTinyInt,adUnsignedBigInt,adUnsignedInt,adUnsignedSmallInt,adUnsignedTinyInt,adVarBinary,adBSTR,adChar,adLongVarChar,adLongVarWChar,adVarChar,adVarWChar,adWChar,adDate,adDBDate,adDBTime,adDBTimeStamp};
	for(int i=0;i<28;i++)
	{
		if(type==tp[i])
			break;
		if(i==27)
			return "未知类型";
	}
	 if(i<18)
		sType="数值型";
	if(i>=18 && i<25)
		sType="字符型";
	if(i>=25 && i<29)
		sType="日期型";
	return sType;
}
