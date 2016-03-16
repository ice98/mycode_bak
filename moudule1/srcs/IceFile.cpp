#include "IceFile.h"
HANDLE IcyCreateFile(
		LPCTSTR lpFileName,
		DWORD dwDesiredAccess, 
		DWORD dwShareMode, 
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile
)					 
{
	HANDLE hFile = CreateFile(
		lpFileName,                       
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes, 
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);

	return hFile;	
}

HANDLE IcyCreateFileMapping( 
	HANDLE hFile,                       // handle to file
	LPSECURITY_ATTRIBUTES lpAttributes, // security
	DWORD flProtect,                    // protection
	DWORD dwMaximumSizeHigh,            // high-order DWORD of size
	DWORD dwMaximumSizeLow ,             // low-order DWORD of size
	LPCTSTR lpName                    // object name
)
{
	HANDLE hMap = CreateFileMapping(
		 hFile,                       // handle to file
		 lpAttributes, // security
		 flProtect,                    // protection
		 dwMaximumSizeHigh,            // high-order DWORD of size
		 dwMaximumSizeLow,             // low-order DWORD of size
		 lpName                      // object name
		);
	return hMap;
}

LPVOID IcyMapViewOfFile(
  HANDLE hFileMappingObject,   // handle to file-mapping object
  DWORD dwDesiredAccess,       // access mode
  DWORD dwFileOffsetHigh,      // high-order DWORD of offset
  DWORD dwFileOffsetLow ,       // low-order DWORD of offset
  SIZE_T dwNumberOfBytesToMap  // number of bytes to map
)
{
	PBYTE pbFile =  NULL;
	pbFile = (PBYTE)MapViewOfFile(
		hFileMappingObject,   // handle to file-mapping object
		dwDesiredAccess,       // access mode
		dwFileOffsetHigh,      // high-order DWORD of offset
		dwFileOffsetLow,       // low-order DWORD of offset
		dwNumberOfBytesToMap  // number of bytes to map
		);

	return pbFile;
}

HERROR IcyMapFile(
				  LPCTSTR lpFileName,    // file name
				  OUT HANDLE *phFile,
				  OUT HANDLE *phMap,
				  OUT LPVOID *pbFile,
				DWORD dwDesiredAccess ,       // access mode
				DWORD dwFileOffsetHigh,      // high-order DWORD of offset
				DWORD dwFileOffsetLow,       // low-order DWORD of offset
				SIZE_T dwNumberOfBytesToMap  // number of bytes to map
				  )
{
	if ((FILE_MAP_COPY == dwDesiredAccess )|| (FILE_MAP_READ== dwDesiredAccess))
	{
		*phFile = IcyCreateFile(lpFileName);
		if (INVALID_HANDLE_VALUE == *phFile)
		{
			return GetLastError();
		}
		*phMap = IcyCreateFileMapping(*phFile);
		{
			if (NULL == *phMap)
			{
				return GetLastError();
			}
		}
		*pbFile = IcyMapViewOfFile(*phMap, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}
	else if ((FILE_MAP_WRITE == dwDesiredAccess) || (FILE_MAP_ALL_ACCESS == dwDesiredAccess))
	{
		*phFile = IcyCreateFile(lpFileName, GENERIC_READ|GENERIC_WRITE);
		if (INVALID_HANDLE_VALUE == *phFile)
		{
			return GetLastError();
		}
		*phMap = IcyCreateFileMapping(*phFile, NULL, PAGE_READWRITE);
		{
			if (NULL == *phMap)
			{
				return GetLastError();
			}
		}
		*pbFile = IcyMapViewOfFile(*phMap, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}
	else
	{
		return FAILE;
	}
	return SUCCESS;
}

HERROR IcyMapFileForOneUse(
	LPCTSTR lpFileName,    // file name
	OUT LPVOID *pbFile,
	DWORD dwDesiredAccess ,       // access mode
	DWORD dwFileOffsetHigh,      // high-order DWORD of offset
	DWORD dwFileOffsetLow ,       // low-order DWORD of offset
	SIZE_T dwNumberOfBytesToMap  // number of bytes to map
)
{
	HANDLE hFile = NULL;
	HANDLE hFileMap = NULL;
	if ((FILE_MAP_COPY == dwDesiredAccess )|| (FILE_MAP_READ== dwDesiredAccess))
	{
		hFile = IcyCreateFile(lpFileName);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return GetLastError();
		}
		hFileMap = IcyCreateFileMapping(hFile);
		{
			if (NULL == hFileMap)
			{
				return GetLastError();
			}	
		}
		*pbFile = IcyMapViewOfFile(hFileMap, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}
	else if ((FILE_MAP_WRITE == dwDesiredAccess) || (FILE_MAP_ALL_ACCESS == dwDesiredAccess))
	{
		hFile = IcyCreateFile(lpFileName, GENERIC_READ|GENERIC_WRITE);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return GetLastError();
		}
		hFileMap = IcyCreateFileMapping(hFile, NULL, PAGE_READWRITE);
		{
			if (NULL == hFileMap)
			{
				return GetLastError();
			}
		}
		*pbFile  = IcyMapViewOfFile(hFileMap, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}
	else
	{
		return FAILE;
	}

	// As file have been successfully mapped, no other use to hFile and hFileMap, close them.
	IcyCloseHandle(hFile, hFileMap);

	return SUCCESS;
}

// Create file map view from system page file, not the disk file,use hFile INVALID_HANDLE_VALUE
HERROR IcyMapFileFromPage(
	OUT LPVOID *pbView,
	DWORD dwDesiredAccess ,       // access mode
	DWORD dwMaximumSizeHigh,            // high-order DWORD of size
	DWORD dwMaximumSizeLow ,
	LPCTSTR lpFileMapName
	)
{
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE, dwMaximumSizeHigh, dwMaximumSizeLow,lpFileMapName);
	if (hMap != NULL)
	{
		*pbView = MapViewOfFile(hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0 ,0);
	}
	else
	{
		ICY_ASSERT0(FAILE);
		return FAILE;
	}

	return SUCCESS;
}

BOOL IcyUnmapViewOfFile(LPCVOID lpBaseAddress)
{
	return UnmapViewOfFile(lpBaseAddress);
}

BOOL IcyCloseHandle(HANDLE hObject1, HANDLE hObject2)
{
	BOOL bRet1 = FALSE;
	BOOL bRet2 = FALSE;
	bRet1 = CloseHandle(hObject1);
	if (NULL != hObject2)
	{
		bRet2 = CloseHandle(hObject2);
	}

	return (bRet1 & bRet2);
}

BOOL IcyFlushViewOfFile(
  LPCVOID lpBaseAddress,         // starting address
  SIZE_T dwNumberOfBytesToFlush  // number of bytes in range
)
{
	return FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
}

HANDLE IcyOpenFileMapping(
	DWORD dwDesiredAccess,  // access mode
	BOOL bInheritHandle,    // inherit flag
	LPCTSTR lpName          // object name
)
{
	return OpenFileMapping(dwDesiredAccess,  bInheritHandle,  lpName);
}


HERROR IcyGetFileSize(HANDLE hFile, DWORD *pdwLow, DWORD *pdwHigh)
{
	DWORD dwFileType = IcyGetFileType(hFile);
	if (!(FILE_TYPE_DISK == dwFileType))
	{
		ICY_ASSERT0(FALSE);
		return FAILE;
	}
	*pdwLow =  GetFileSize(hFile, pdwHigh);

	if (INVALID_FILE_SIZE == *pdwLow)
	{
		ICY_ASSERT0(FALSE);
		return FAILE;
	}

	return SUCCESS;	
}

/*
FILE_TYPE_UNKNOWN The type of the specified file is unknown. 
FILE_TYPE_DISK The specified file is a disk file. 
FILE_TYPE_CHAR The specified file is a character file, typically an LPT device or a console. 
FILE_TYPE_PIPE The specified file is either a named or anonymous pipe. 
*/
DWORD IcyGetFileType(
  HANDLE hFile   // handle to file
)
{
	ICY_ASSERT0(NULL != hFile);
	return GetFileType(hFile);
}

CFileMap::CFileMap(string strFilePathName,  DWORD dwAccessModeOfView, DWORD dwNumberOfBytesToMap
				   , DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh)
 {
	m_pbAddrsStartMap = NULL;
	m_strFilePathName = strFilePathName;
	m_dwFileMapViewDesiredAccess = dwAccessModeOfView;
	if (FILE_MAP_WRITE == m_dwFileMapViewDesiredAccess
		|| FILE_MAP_ALL_ACCESS == m_dwFileMapViewDesiredAccess)
	{
		m_dwFileDesiredAccess = (GENERIC_READ | GENERIC_WRITE);
		m_dwFileMapDesiredAccess = PAGE_READWRITE;
	}
	else if(FILE_MAP_READ == m_dwFileMapViewDesiredAccess)
	{
		m_dwFileDesiredAccess = GENERIC_READ;
		m_dwFileMapDesiredAccess = PAGE_READONLY;
	}
	else if(FILE_MAP_COPY == m_dwFileMapViewDesiredAccess)
	{
		m_dwFileDesiredAccess = (GENERIC_READ | GENERIC_WRITE);
		m_dwFileMapDesiredAccess = PAGE_WRITECOPY;
	}

	m_dwFileShareMode = FILE_SHARE_READ;

	m_dwMaximumSizeHigh = 0;           
	m_dwMaximumSizeLow = 0;
	
	m_dwFileOffsetHigh = dwFileOffsetHigh;      
	m_dwFileOffsetLow = dwFileOffsetLow;     
	m_dwNumberOfBytesToMap = dwNumberOfBytesToMap; 

	m_hFile = IcyCreateFile(m_strFilePathName.c_str(), m_dwFileDesiredAccess, m_dwFileShareMode);

	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		m_hFileMap = IcyCreateFileMapping(m_hFile, NULL, m_dwFileMapDesiredAccess,
			m_dwMaximumSizeLow, m_dwMaximumSizeLow, m_lpFileMapObjName);
	}

	if (NULL != m_hFileMap)
	{
		m_pbAddrsStartMap = (PBYTE)IcyMapViewOfFile(m_hFileMap, m_dwFileMapViewDesiredAccess
			, m_dwFileOffsetHigh, m_dwFileOffsetLow, m_dwNumberOfBytesToMap);
	}	
}


CFileMap::~CFileMap()
{
	IcyUnmapViewOfFile(m_pbAddrsStartMap);
	IcyCloseHandle(m_hFile, m_hFileMap);
}

HERROR CFileMap::WriteToMapFile(LPBYTE lpStartAddr, const LPBYTE lpBuffer, DWORD dwSize, BOOL bFlushSoon)
{
	memcpy(lpStartAddr, lpBuffer, dwSize);
	if (bFlushSoon)
	{
		FlushView(lpStartAddr, dwSize);
	}
	return SUCCESS;
}

HERROR CFileMap::FlushView(LPBYTE lpAddr, DWORD dwBytesToFlush)
{
	IcyFlushViewOfFile(lpAddr, dwBytesToFlush);
	return SUCCESS;
}

HERROR CFileMap::GetMapViewSize(HANDLE hFile, DWORD *pdwLow, DWORD *pdwHigh)
{
	if (0 == m_dwNumberOfBytesToMap
		&& 0 == m_dwFileOffsetLow
		&& 0 == m_dwFileOffsetHigh)
	{
		HERROR hRet = IcyGetFileSize(hFile, pdwLow, pdwHigh);
		ICY_ASSERT0(FALSE);
		return hRet;
	}
	else
	{
		*pdwLow = m_dwNumberOfBytesToMap;
		*pdwHigh = 0;
	}

	return SUCCESS;
}

HERROR CFileMap::GetMapOffset(DWORD &pdwLow, DWORD &pdwHigh)
{
	pdwLow = m_dwFileOffsetLow;
	pdwHigh = m_dwFileOffsetHigh;
	return SUCCESS;
}

