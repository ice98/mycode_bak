// RxADO.cpp: implementation of the RxADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RxADO.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
_ConnectionPtr cnn;
CString OP;
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
			MessageBox(NULL,"��������:\n\n"+ErrMsg,"ϵͳ��ʾ",MB_OK|MB_ICONEXCLAMATION);	
			return false;
		}
		else
		{
			for (int i=0;i<pErrors->GetCount();i++)
			{
				_bstr_t desc=pErrors->GetItem((long)i)->GetDescription();
				MessageBox(NULL,"��������:\n\n"+desc,"ϵͳ��ʾ",MB_OK|MB_ICONEXCLAMATION);
				return false;
			}
		}	
	}
	return true;
}
/*
 ��nStale=1ʱ �ַ��ֶη����ַ��ֶ�
 ��nStale=2ʱ ��ֵ�ֶη����ַ��ֶ�
 ��nStale=3ʱ �����ֶη����ַ��ֶ�
*/
CString RxADO::FieldToOtherField(CString cDataBaseName, CString cFieldName, CString cValue, CString cReturnField, int nStale=1)
{
	CString sSQL,sRTValue;
	_RecordsetPtr Fieldrst;
	Fieldrst.CreateInstance(__uuidof(Recordset));
	Fieldrst.CreateInstance(__uuidof(Recordset));
	if(nStale==1)	//�ַ��ֶη����ַ��ֶ�
			sSQL.Format("SELECT * FROM %s WHERE %s='%s'",cDataBaseName,cFieldName,cValue);
	if(nStale==2)//��ֵ�ֶη����ַ��ֶ�
			sSQL.Format("SELECT * FROM %s WHERE %s=%s",cDataBaseName,cFieldName,cValue);
	if(nStale==3)//�����ֶη����ַ��ֶ�
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
		MessageBox(NULL,eErrors.ErrorMessage(),"��  ��",MB_OK|MB_ICONEXCLAMATION);	
	else
	{
		for (int i=0;i<pErrors->GetCount();i++)
		{
			_bstr_t desc=pErrors->GetItem((long)i)->GetDescription();
			MessageBox(NULL,desc,"��  ��",MB_OK|MB_ICONEXCLAMATION);
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
	��nStyle=1ʱ �������ֱ��
	��nStyle=2ʱ ������ˮ�ʺ�
	��nStyle=3ʱ ��������ʱ����
*/
CString RxADO::AutoNumber(CString sTable, CString sFieldName, CString sCode, int nStyle)
{
 	return "";
}




_ConnectionPtr RxADO::GetConnection()
{
	return cnn;
}

void RxADO::Close()
{
	cnn->Close();
}
