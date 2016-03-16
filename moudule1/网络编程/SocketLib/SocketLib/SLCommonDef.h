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

#define  MSG_MAXPARAMS       8		//����ͷ�в�����������
#define  MSG_HEAD_SIZE      40      //����ͷ�Ĵ�С
#define  MAX_KEY_LEN		16      //�����Կ����
#define  MAX_USERNAME_LEN   32		//����û�������
#define  MAX_PTNAME_LEN		20      //���PTName����
/********************************
*    �������ݽṹ
********************************/ 
//����ͷ�Ľṹ
typedef struct tag_SL_MSGHEAD
{
	ulong			nSize;						//�ܳ��� = Э��ͷ �� Э������(������)
	ulong			nCommand;					//����
	int				nParams[MSG_MAXPARAMS];		//����
}SL_MSGHEAD;
typedef struct tag_SL_MSG
{
	SL_MSGHEAD		msghead;
	char			msgcontent[1];
}SL_MSG;

//����ͷ�ṹ��һ�ֱ���
typedef struct tag_SL_MSGHEAD_EX
{
	ulong			nSize;						//�ܳ��� = Э��ͷ �� Э������(������)
	ulong			nCommand;					//����
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
	uint16			nSize;						//�ܳ��� = Э��ͷ �� Э������(������)
	uint16			nCommand;
}SL_MSGHEAD_SMALL;
typedef struct tag_SL_MSGSMALL
{
	SL_MSGHEAD_SMALL msghead;
	char			 msgcontent[1];
}SL_MSG_SMALL;

enum SL_MSG_COMMAND_FLAG
{
	SL_MSG_COMMAND_FLAG_ENCRYPT   = 0x0001,  //����
	SL_MSG_COMMAND_FLAG_COMPRESS  = 0x0002   //ѹ��
};

};

#endif
