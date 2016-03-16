// RxRecordset.h: interface for the RxRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RXRECORDSET_H__3D949DD0_3A07_4BFC_B7A7_821CB3AA794F__INCLUDED_)
#define AFX_RXRECORDSET_H__3D949DD0_3A07_4BFC_B7A7_821CB3AA794F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RxRecordset  
{
public:
	CString GetFieldType(int nNumber);
	CString GetSQL();
	CString GetFieldName(int nNumber);
	void Close();
	void GetErrors(_com_error eErrors);
	_RecordsetPtr GetRecordset();
	int GetFieldCount();
	bool Move(int nRecordNumber);
	bool MovePrevious();
	bool MoveLast();
	bool MoveNext();
	bool MoveFirst();
	int GetRecordCount();
	bool Open(CString srecordset, UINT adCmd=adCmdTable);
	CString GetFieldValue(CString Field);
	RxRecordset();
	virtual ~RxRecordset();

private:
	CString sRecordset;
	_RecordsetPtr rst;
};

#endif // !defined(AFX_RXRECORDSET_H__3D949DD0_3A07_4BFC_B7A7_821CB3AA794F__INCLUDED_)
