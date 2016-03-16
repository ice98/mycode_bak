/*****************************************************************************
*@File:Icy.h
*@Author:Icy
*@Date:Mar,2008
*/
#ifndef _ICY_H_
#define _ICY_H_

#include "IceErr.h"
#include "IcePub.h"

#define ERROR_PRINT   (1)
#define ERROR_RECORD  (2)
#define ERROR_PRINT_RECORD  (3)

#define ERROR_CATCH_MODE ERROR_PRINT

#if(ERROR_CATCH_MODE == ERROR_PRINT)
	#define ICY_ASSERT0(con) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE) : 0)

	#define ICY_ASSERT1(con, msg) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE, msg) : 0)

	#define ICY_ASSERT2(con, code) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE, NULL, code) : 0)

	#define ICY_ASSERT3(con, msg, code) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE, NULL, msg, code) : 0)

#elif(ERROR_CATCH_MODE == ERROR_RECORD)
	#define ICY_ASSERT0(con) \
	(!(con) ? RecordError(ICY_FILE, ICY_LINE) : 0)

	#define ICY_ASSERT1(con, msg) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE, msg) : 0)
#elif(ERROR_CATCH_MODE == ERROR_PRINT_RECORD)
	#define ICY_ASSERT0(con) \
	(!(con) ? RecordErrorWithPrint(ICY_FILE, ICY_LINE) : 0)

	#define ICY_ASSERT1(con, msg) \
	(!(con) ? PrintError(ICY_FILE, ICY_LINE, msg) : 0)
#else
	#define ICY_ASSERT0(con)
	#define ICY_ASSERT1(con, msg)
	#define ICY_ASSERT2(con, code)
	#define ICY_ASSERT3(con, msg, code)

#endif
/////////////////////////////////////////////////////////////////////////////
#define _DLL_EXPORT  extern "C" __declspec(dllexport)
#define _DLL_IMPORT  extern "C" __declspec(dllimport)

#if defined(PROJ_DLL)
	#define ICY_DLL _DLL_EXPORT
#elif defined(PROJ_APP)
	#define ICY_DLL _DLL_IMPORT
#else
	#define ICY_DLL
#endif

#endif
