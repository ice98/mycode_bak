/* !@IceSocket.h
********************************************************************************
ģ����		: 
�ļ���		: IceSocket.h
����ļ�	: IceSocket.cpp
�ļ�����	:
����		: Icy
�汾		: 1.0
--------------------------------------------------------------------------------
��ע		: 
--------------------------------------------------------------------------------
�޸ļ�¼ : 
��  ��          �汾            �޸���          �޸����� 
2008/03/20     1.0             Icy				����
********************************************************************************
* ��Ȩ����(c) 2008, 2015, Icy, ��������Ȩ��
================================================================================
 
*******************************************************************************/

#ifndef _ICE_SOCKET_H
#define _ICE_SOCKET_H
#include "Ice.h"

#if(ERROR_CATCH_MODE == ERROR_PRINT)
	#define SOCK_ASSERT0(con) (!(con) ? Trace0(ICY_FILE, ICY_LINE) : 0) 
#else
	#define SOCK_ASSERT0(con)
#endif

HERROR Trace0(string _file_path_, DWORD _line_no_);
HERROR InitSocket(BYTE ver_high, BYTE ver_low);
HERROR BindSocket(SOCKET socket, const CHAR* sAddr, WORD wPort);
HERROR ListenSocket(SOCKET socket, int backlog);
HERROR AcceptSocket(SOCKET socket, SOCKADDR *addr, int *addr_len, SOCKET *socket_new);
HERROR ConnectSocket(SOCKET socket, const CHAR* sAddr, WORD wPort);
HERROR RecvFromSocket(SOCKET socket, CHAR* buf, int len, int flags, int *rcv_len);
HERROR SendToSocket(SOCKET socket, CHAR* buf, int len, int flags);

#endif
