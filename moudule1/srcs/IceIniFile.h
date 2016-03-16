#ifndef __ICE_INIFILE_H__
#define __ICE_INIFILE_H__
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#include <map>
#include <list>
#include <iostream>
#include <string>
using namespace std;


// 对外提供接口
extern unsigned long GetIniFileString(const char *  lpSecName,        // section name
                             const char *  lpKeyName,        // key name
                             const char *  lpDefault,        // default string
                             char *  lpReturnedString,       // destination buffer
                             unsigned long nSize,            // size of destination buffer
                             const char *  lpFileName        // initialization file name
                             );
extern unsigned long WriteIniFileString(
                               const char *  lpSecName,        // section name
                               const char *  lpKeyName,        // key name
                               const char *  lpKeyValue,       // default string
                               const char *  lpFileName        // initialization file name
                               );

class CIniFile;
class CIniOpera
{
public:
    static CIniFile* GetIniOperIns(string strFileName);
    virtual ~CIniOpera();
private:
    static list<CIniFile*> m_lstIniFile;
    CIniOpera();
    CIniOpera(CIniOpera& rhs);
    CIniOpera& operator=(CIniOpera& rhs);
};

class CIniFile
{
public:
	CIniFile(const string &filename);
	CIniFile(const char * buf, unsigned long size)
	{
		GetIniInfoFromFileBuf(buf,size);
	}
	virtual ~CIniFile();
	long GetIniFileInt(
		const char * lpSecName,  // section name
		const char * lpKeyName,  // key name
		long nDefault,           // return value if key name not found
		const char * lpFileName  // initialization file name
		);
	
	unsigned long GetIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpDefault,        // default string
		char *  lpReturnedString,       // destination buffer
		unsigned long nSize,            // size of destination buffer
		const char *  lpFileName        // initialization file name
		);
	
	long GetIniFileInt(
		const char * section,  // section name
		const char * key,      // key name
		long defaultvalue      // return value if key name not found
		);	

	unsigned long GetIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpDefault,        // default string
		char *  lpReturnedString,       // destination buffer
		unsigned long nSize,            // size of destination buffer
		);

    void WriteIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpKeyValue        // default string
		);

    void WriteIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpKeyValue,        // default string
		const char *  lpFileName        // initialization file name
		);
    unsigned long RefreshIniFile();

    string GetIniFilePathName()
    {
        return m_strFilePathName;
    }

private:
	void GetIniInfoFromFileBuf(const char * buf, unsigned long size);
	typedef map<const string, string> KEY_VALUE_MAP;
	typedef map<const string, KEY_VALUE_MAP> SECTION_KEY_MAP;
	SECTION_KEY_MAP m_mapmapIniBuf;
    string m_strFilePathName;
    FILE *m_pFp;
    enum STATE {ENTER_SECTION,END_SECTION, ENTER_KEY, END_KEY, ENTER_STRING, END_STRING,START};
};
#endif
