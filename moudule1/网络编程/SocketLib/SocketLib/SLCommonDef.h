//**********************************************************************
//
// Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com).
// All rights reserved.
//
// This copy of Socketlib is licensed to you under the terms described 
// in the LICENSE.txt file included in this distribution.
//
//**********************************************************************

#ifndef SL_COMMON_DEF_H
#define SL_COMMON_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "slbasetype.h"

namespace SL
{

#define  MSG_MAXPARAMS       8		//报文头中参数的最大个数
#define  MSG_HEAD_SIZE      40      //报文头的大小
#define  MAX_KEY_LEN		16      //最大密钥长度
#define  MAX_USERNAME_LEN   32		//最大用户名长度
#define  MAX_PTNAME_LEN		20      //最大PTName长度
/********************************
*    定义数据结构
********************************/ 
//报文头的结构
typedef struct tag_SL_MSGHEAD
{
	ulong			nSize;						//总长度 = 协议头 ＋ 协议正文(不定长)
	ulong			nCommand;					//命令
	int				nParams[MSG_MAXPARAMS];		//参数
}SL_MSGHEAD;
typedef struct tag_SL_MSG
{
	SL_MSGHEAD		msghead;
	char			msgcontent[1];
}SL_MSG;

//报文头结构的一种变体
typedef struct tag_SL_MSGHEAD_EX
{
	ulong			nSize;						//总长度 = 协议头 ＋ 协议正文(不定长)
	ulong			nCommand;					//命令
	int				nParams[6];
	int64			nAttachID;
}SL_MSGHEAD_EX;
typedef struct tag_SL_MSG_EX
{
	SL_MSGHEAD_EX	msghead;
	char			msgcontent[1];
}SL_MSG_EX;

typedef struct tag_SL_MSGHEAD_SMALL
{
	uint16			nSize;						//总长度 = 协议头 ＋ 协议正文(不定长)
	uint16			nCommand;
}SL_MSGHEAD_SMALL;
typedef struct tag_SL_MSGSMALL
{
	SL_MSGHEAD_SMALL msghead;
	char			 msgcontent[1];
}SL_MSG_SMALL;

enum SL_MSG_COMMAND_FLAG
{
	SL_MSG_COMMAND_FLAG_ENCRYPT   = 0x0001,  //加密
	SL_MSG_COMMAND_FLAG_COMPRESS  = 0x0002   //压缩
};

};

#endif
