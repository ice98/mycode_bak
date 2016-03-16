/* !@IceSocket.h
********************************************************************************
模块名		: 
文件名		: IceSocket.h
相关文件	: IceSocket.cpp
文件描述	:
作者		: Icy
版本		: 1.0
--------------------------------------------------------------------------------
备注		: 
--------------------------------------------------------------------------------
修改记录 : 
日  期          版本            修改人          修改内容 
2008/03/20     1.0             Icy				创建
********************************************************************************
* 版权所有(c) 2008, 2015, Icy, 保留所有权利
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
