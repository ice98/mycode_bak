#ifndef _ICE_FILE_
#define _ICE_FILE_
#include "Ice.h"

/* dwShareMode:告诉系统你想如何共享该文件.
0 打开文件的任何尝试均将失败
FILE_SHARE_READ 使用GENERIC_WRITE 打开文件的其他尝试将会失败
FILE_SHARE_WRITE 使用GENERIC_READ 打开文件的其他尝试将会失败
FILE_SHARE_READ | FILE_SHARE_WRITE,打开文件的其他尝试将会取得成功
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
FILE_MAP_WRITE可以读取和写入文件数据。CreateFileMapping函数必须通过传递PAGE_READWRITE标志来调用.
FILE_MAP_READ可以读取文件数据。CreateFileMapping函数可以通过传递下列任何一个保护属性来调用：PAGE_READONLY、PAGE_READWRITE或PAGE_WRITECOPY.
FILE_MAP_ALL_ACCESS与FILE_MAP_WRITE相同.
FILE_MAP_COPY可以读取和写入文件数据。如果写入文件数据，可以创建一个页面的私有拷贝。在Windows2000中，CreateFileMapping函数可以用PAGE_READONLY、PAGE_READWRITE或PAGE_WRITECOPY等保护属性中的任何一个来调用。在Windows98中，CreateFileMapping必须用PAGE_WRITECOPY来调用
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

/*与 IcyMapFile 不同的是内部自动关掉文件对象和文件映射对象*/
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