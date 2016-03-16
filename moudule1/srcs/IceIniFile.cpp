#include "IceIniFile.h"
// �����ṩ�ӿ�
unsigned long GetIniFileString(const char *  lpSecName,        // section name
                      const char *  lpKeyName,        // key name
                      const char *  lpDefault,        // default string
                      char *  lpReturnedString,       // destination buffer
                      unsigned long nSize,            // size of destination buffer
                      const char *  lpFileName        // initialization file name
                      )
{
    CIniFile* pIniFile = CIniOpera::GetIniOperIns(lpFileName);
    pIniFile->GetIniFileString(
                                lpSecName,  
                                lpKeyName,        
		                        lpDefault,        
		                        lpReturnedString,      
		                        nSize);
}
unsigned long WriteIniFileString(
                        const char *  lpSecName,        // section name
                        const char *  lpKeyName,        // key name
                        const char *  lpKeyValue,       // default string
                        const char *  lpFileName        // initialization file name
                        )
{
    CIniFile* pIniFile = CIniOpera::GetIniOperIns(lpFileName);
    pIniFile->WriteIniFileString(lpSecName, lpKeyName, lpKeyValue);
}

list<CIniFile*> CIniOpera::m_lstIniFile;
CIniFile* CIniOpera::GetIniOperIns(string strFileName)
{
    list<CIniFile*>::iterator lstIter = m_lstIniFile.begin();
    for (; lstIter != m_lstIniFile.end(); lstIter++)
    {
        if (strFileName == (*lstIter)->GetIniFilePathName())
        {
            return *lstIter;
        }
    }
    CIniFile *pIniFile = new CIniFile(strFileName);
    m_lstIniFile.push_back(pIniFile);
    return pIniFile;
}

CIniOpera::~CIniOpera()
{
    list<CIniFile*>::iterator lstIter = m_lstIniFile.begin();
    for (; lstIter != m_lstIniFile.end(); lstIter++)
    {
        delete *lstIter;
    }
}

CIniFile::CIniFile(const string &filename)
{
    //���ļ���ȡ��
    m_strFilePathName = filename;
    //ע�⣬�˴�ģʽֻ����r+,��������ʵ�顣
    m_pFp = fopen(m_strFilePathName.c_str(),"r+");
    if(m_pFp)
    {
        fseek(m_pFp,0,SEEK_END);
        int size = ftell(m_pFp);
        char * p = new char[size+1];
        p[size] = 0;
        fseek(m_pFp, 0, SEEK_SET);
        fread(p,size,1,m_pFp);
        GetIniInfoFromFileBuf(p,size);
        delete [] p;
    }
    else
    {
        printf("no file opened!\n");
    }
}

void CIniFile::GetIniInfoFromFileBuf(const char * buf, unsigned long size)
{
    STATE s = START;
    string section;
    string key;
    string str;
    while ( *buf != '\0' )
    {
        switch( s ) 
        {
        case START:
            {
                if( *buf == '[')
                {
                    section = "";
                    s = ENTER_SECTION;
                }
                break;
            }
        case ENTER_SECTION:
            {
                if( *buf == ']') 
                {
                    s = END_SECTION;
                }
                else 
                {
                    section += *buf;
                }
                break;
            }
        case END_SECTION:
            {
                if( *buf == '\n' ) //���У���ʾ����key������
                {
                    key = "";
                    s = ENTER_KEY;
                }
                break;
            }
        case ENTER_KEY:
            {
                if( *buf == '=' )
                {
                    str = "";
                    s = END_KEY;
                }
                else
                {
                    if( *buf != ' ' 
                        && *buf != '\t'
                        && *buf != '\r'
                        && *buf != '\n'
                        )
                    {
                        key += *buf;
                    }
                }
                break;
            }
        case END_KEY:
            {
                if( *buf != ' ' 
                    && *buf != '\t'
                    && *buf != '\r'
                    && *buf != '\n')
                {
                    str = *buf;
                    s = ENTER_STRING;
                }
                break;
            }
        case ENTER_STRING:
            {
                if( *buf != '\n' ) //û�л���
                {
                    if( *buf != '\r' )
                        str += *buf;
                }
                else //��������
                {
                    m_mapmapIniBuf[section][key] = str;
                    s = END_STRING;
                }
                break;
            }
        case END_STRING: //��һ���п�������sectionҲ�п�������key������key�����Щ��
            {
                switch(*buf) 
                {
                case '[': //�µ�section
                    {
                        section = "";
                        s = ENTER_SECTION;
                        break;
                    }
                case ' ' :
                case '\r':
                case '\n':
                case '\t':
                    {
                        break;//���ԡ�
                    }
                default: //��������ǣ�����key�ˡ�
                    {
                        key = *buf;
                        s = ENTER_KEY;
                    }
                }
                break;
            }
        default:
            {
                break;
            }

        }
        buf++;
    }    
}

CIniFile::~CIniFile()
{
    RefreshIniFile();
    fclose(m_pFp);
}


unsigned long CIniFile::GetIniFileString(const char *  section,         // section name
                                         const char *  key,             // key name
                                         const char *  defaultstring,   // default string
                                         char *  destination,           // destination buffer
                                         unsigned long size             // size of destination buffer
                                         )
{
    string ret = m_mapmapIniBuf[string(section)][ string(key) ];
    if( ret.length() == 0 ) //û��
    {
        ret = defaultstring;
    }
    strncpy(destination,ret.c_str(),size);
    return ret.length();
}

long CIniFile::GetIniFileInt(
                             const char * section,  // section name
                             const char * key,      // key name
                             long defaultvalue      // return value if key name not found
                             )
{
    string ret = m_mapmapIniBuf[string(section)][string(key)];
    if( ret.length() == 0 ) //û��
    {
        return defaultvalue;
    }
    else
    {
        return atoi(ret.c_str());
    }
}

long CIniFile::GetIniFileInt(
                             const char * lpAppName,    // section name
                             const char * lpKeyName,    // key name
                             long nDefault,             // return value if key name not found
                             const char * lpFileName    // initialization file name
                             )
{
    return CIniFile(string(lpFileName)).GetIniFileInt(lpAppName,lpKeyName,nDefault);
}

unsigned long CIniFile::GetIniFileString(
                                         const char *  lpAppName,        // section name
                                         const char *  lpKeyName,        // key name
                                         const char *  lpDefault,        // default string
                                         char *  lpReturnedString,       // destination buffer
                                         unsigned long nSize,            // size of destination buffer
                                         const char *  lpFileName        // initialization file name
                                         )
{
    return CIniFile(string(lpFileName)).GetIniFileString(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize);
}

void CIniFile::WriteIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpKeyValue        // default string
		)
{
     m_mapmapIniBuf[lpSecName][lpKeyName] = lpKeyValue;
}

void CIniFile::WriteIniFileString(
		const char *  lpSecName,        // section name
		const char *  lpKeyName,        // key name
		const char *  lpKeyValue,       // default string
		const char *  lpFileName        // initialization file name
		)
 {
     CIniFile(lpFileName).WriteIniFileString(lpSecName, lpKeyName, lpKeyValue);
 }


unsigned long CIniFile::RefreshIniFile()
{

    if (NULL == m_pFp)
    {
         m_pFp = fopen(m_strFilePathName.c_str(),"a+");
         if (NULL == m_pFp)
         {
             return 1;
         }
    }

    fseek(m_pFp, 0, SEEK_SET);
    SECTION_KEY_MAP::const_iterator iteri;
    KEY_VALUE_MAP::const_iterator iterj;
    string strSecItem;
    string strKeyItem;

    for (iteri = m_mapmapIniBuf.begin(); iteri != m_mapmapIniBuf.end(); iteri++)
    {
        strSecItem = "\n["+ iteri->first + "]\n";
        fputs(strSecItem.c_str(), m_pFp);
        for (iterj = iteri->second.begin(); iterj != iteri->second.end(); iterj++)
        {
            strKeyItem = iterj->first + "=" + iterj->second + "\n";
            fputs(strKeyItem.c_str(), m_pFp);
        }
    }

    fflush(m_pFp);
    return 0;
}

