#ifndef _ICE_FILE_
#define _ICE_FILE_
#include "Ice.h"

/* dwShareMode:����ϵͳ������ι�����ļ�.
0 ���ļ����κγ��Ծ���ʧ��
FILE_SHARE_READ ʹ��GENERIC_WRITE ���ļ����������Խ���ʧ��
FILE_SHARE_WRITE ʹ��GENERIC_READ ���ļ����������Խ���ʧ��
FILE_SHARE_READ | FILE_SHARE_WRITE,���ļ����������Խ���ȡ�óɹ�
*/
HANDLE IcyCreateFile(
	LPCTSTR lpFileName,    // file name
	DWORD dwDesiredAccess = GENERIC_READ,     // access mode
	DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, // SD
	DWORD dwCreationDisposition = OPEN_ALWAYS,         // how to create
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,// file attributes
	HANDLE hTemplateFile = NULL
);					 

/*
while flProtect = PAGE_READONLY, be sure dwDesiredAccess = GENERIC_READ in CreateFile.
while flProtect = PAGE_READWRITE or PAGE_WRITECOPY, be sure dwDesiredAccess = GENERIC_READ
|GENERIC_WRITE in CreateFile.
*/
HANDLE IcyCreateFileMapping(
	HANDLE hFile,                       // handle to file
	LPSECURITY_ATTRIBUTES lpAttributes = NULL, // security
	DWORD flProtect = PAGE_READONLY,                    // protection
	DWORD dwMaximumSizeHigh = 0,            // high-order DWORD of size
	DWORD dwMaximumSizeLow = 0,             // low-order DWORD of size
	LPCTSTR lpName = NULL                    // object name
);

/*
FILE_MAP_WRITE���Զ�ȡ��д���ļ����ݡ�CreateFileMapping��������ͨ������PAGE_READWRITE��־������.
FILE_MAP_READ���Զ�ȡ�ļ����ݡ�CreateFileMapping��������ͨ�����������κ�һ���������������ã�PAGE_READONLY��PAGE_READWRITE��PAGE_WRITECOPY.
FILE_MAP_ALL_ACCESS��FILE_MAP_WRITE��ͬ.
FILE_MAP_COPY���Զ�ȡ��д���ļ����ݡ����д���ļ����ݣ����Դ���һ��ҳ���˽�п�������Windows2000�У�CreateFileMapping����������PAGE_READONLY��PAGE_READWRITE��PAGE_WRITECOPY�ȱ��������е��κ�һ�������á���Windows98�У�CreateFileMapping������PAGE_WRITECOPY������
*/
LPVOID IcyMapViewOfFile(
  HANDLE hFileMappingObject,   // handle to file-mapping object
  DWORD dwDesiredAccess = FILE_MAP_COPY,       // access mode
  DWORD dwFileOffsetHigh = 0,      // high-order DWORD of offset
  DWORD dwFileOffsetLow = 0,       // low-order DWORD of offset
  SIZE_T dwNumberOfBytesToMap = 0  // number of bytes to map
);

HERROR IcyMapFile(
	LPCTSTR lpFileName,    // file name
	OUT HANDLE *phFile,
	OUT HANDLE *phMap,
	OUT LPVOID *pbFile,
	DWORD dwDesiredAccess = FILE_MAP_COPY,       // access mode
	DWORD dwFileOffsetHigh = 0,      // high-order DWORD of offset
	DWORD dwFileOffsetLow = 0,       // low-order DWORD of offset
	SIZE_T dwNumberOfBytesToMap = 0  // number of bytes to map
);

/*�� IcyMapFile ��ͬ�����ڲ��Զ��ص��ļ�������ļ�ӳ�����*/
HERROR IcyMapFileForOneUse(
	LPCTSTR lpFileName,    // file name
	OUT LPVOID *pbFile,
	DWORD dwDesiredAccess = FILE_MAP_COPY,       // access mode
	DWORD dwFileOffsetHigh = 0,      // high-order DWORD of offset
	DWORD dwFileOffsetLow = 0,       // low-order DWORD of offset
	SIZE_T dwNumberOfBytesToMap = 0  // number of bytes to map
);

// Create file map view from system page file, not the disk file,use hFile INVALID_HANDLE_VALUE
HERROR IcyMapFileFromPage(
	OUT LPVOID *pbView,
	DWORD dwDesiredAccess ,       // access mode
	DWORD dwMaximumSizeHigh,            // high-order DWORD of size
	DWORD dwMaximumSizeLow ,
	LPCTSTR lpFileMapName
	);
BOOL IcyUnmapViewOfFile(LPCVOID lpBaseAddress);
BOOL IcyCloseHandle(HANDLE hObject1, HANDLE hObject2 = NULL);
BOOL IcyFlushViewOfFile(
  LPCVOID lpBaseAddress,         // starting address
  SIZE_T dwNumberOfBytesToFlush  // number of bytes in range
);

HANDLE IcyOpenFileMapping(
  LPCTSTR lpName,         // object name
  DWORD dwDesiredAccess = FILE_MAP_WRITE,  // access mode
  BOOL bInheritHandle = TRUE    // inherit flag
);

HERROR IcyGetFileSize(HANDLE hFile, DWORD *pdwLow, DWORD *pdwHigh = NULL);
DWORD IcyGetFileType( HANDLE hFile);

class CFileMap
{
public:
	CFileMap(string strFilePathName,  DWORD dwAccessModeOfView = FILE_MAP_WRITE,
		DWORD dwNumberOfBytesToMap = 0, DWORD dwFileOffsetLow = 0, 	DWORD dwFileOffsetHigh = 0);
	~CFileMap();

	string GetFileName() {return m_strFilePathName;}
	HERROR GetMapOffset(DWORD &pdwLow, DWORD &pdwHigh);
	PBYTE GetMapAddress() {return m_pbAddrsStartMap;}
	HERROR WriteToMapFile(LPBYTE lpStartAddr, const LPBYTE lpBuffer, DWORD dwSize, BOOL bFlushSoon = FALSE);
	HERROR FlushView(LPBYTE lpAddr, DWORD dwBytesToFlush);
	HERROR GetMapViewSize(HANDLE hFile, DWORD *pdwLow, DWORD *pdwHigh);
protected:
private:
	CFileMap();
	string m_strFilePathName;
	/* Max byte size expected to reserved,if for read or won't change the file's size, set both to zero,
	if let it be real file size, set both to zero.
	PAGE_READWRITE or PAGE_WRITECOPY*/
	DWORD m_dwMaximumSizeHigh;            // high-order DWORD of size
	DWORD m_dwMaximumSizeLow;             // low-order DWORD of size
	
	/* Used to show from where of the file to be map as the first byte, and how many bytes to be mapped,
	set all to zero will map the total file.	*/
	DWORD m_dwFileOffsetHigh;      // high-order DWORD of offset
	DWORD m_dwFileOffsetLow;       // low-order DWORD of offset
	SIZE_T m_dwNumberOfBytesToMap; // number of bytes to map

	HANDLE m_hFile;
	HANDLE m_hFileMap;

	DWORD m_dwFileShareMode;
	DWORD m_dwFileDesiredAccess; 
	DWORD m_dwFileMapDesiredAccess; 
	DWORD m_dwFileMapViewDesiredAccess; 
	PBYTE m_pbAddrsStartMap;
	LPCTSTR m_lpFileMapObjName; // Name of file map object,usually not needed, set to null. 

};

#endif