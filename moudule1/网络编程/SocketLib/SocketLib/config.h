//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOCKETLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOCKETLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef SOCKETLIB_CONFIG_H
#define SOCKETLIB_CONFIG_H
#pragma once

#ifdef SOCKETLIB_EXPORTS
	#define SOCKETLIB_API __declspec(dllexport)
#else
	#define SOCKETLIB_API __declspec(dllimport)
#endif

// Winsock库版本(默认winsock2)
#if !defined(IS_USE_WINSOCK2)
	#define IS_USE_WINSOCK2 1
#endif

#if defined (IS_USE_WINSOCK2) && (IS_USE_WINSOCK2 != 0)
	#define SL_WINSOCK_VERSION 2, 2
	// will also include windows.h, if not present
	#include <winsock2.h>
#else
	#define SL_WINSOCK_VERSION 1, 0
	#include <winsock.h>
#endif
#pragma comment(lib, "ws2_32.lib")

// 微软扩展SOCKET API
#if !defined(_MSWSOCK_)
	#include <mswsock.h>
	#pragma comment(lib, "mswsock.lib")
#endif

// 系统日志
//#define SHOW_PRINTFINFO
#ifndef SOCKETLIB_WRITE_NOLOG
#define SOCKETLIB_WRITE_LOG
#endif

#ifdef UNICODE
#define SOCKETLIB_HAS_WCHAR
#endif
#ifdef _UNICODE
#define SOCKETLIB_HAS_WCHAR
#endif

// 常用头文件
#include "SLBaseType.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#ifndef assert
#include <assert.h>
#endif

#if !defined(SOCKETLIB_HAS_WCHAR)
typedef char TCHAR;
typedef std::string TSTRING;
#else
typedef wchar_t TCHAR;
typedef std::wstring TSTRING;
#endif

#if defined(ASSERT)
#define SL_ASSERT(e)		ASSERT(e)
#elif defined(_ASSERTE)
#define SL_ASSERT(e)		_ASSERTE(e)
#else
#ifdef _DEBUG
#define SL_ASSERT(e)		assert(e)
#else
#define SL_ASSERT(e)
#endif
#endif

#endif