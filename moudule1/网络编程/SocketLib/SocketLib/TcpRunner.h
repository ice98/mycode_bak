/*
* Copyright (C) 2005-2007 Zhang bao yuan(bolidezhang@gmail.com)
* All rights reserved.
* 
* 文件名称：TcpRunner.h
* 摘    要：TcpRunner
* 作    者：张保元 bolidezhang@gmail.com
* 完成日期：2005/07/16
*
*/

#pragma once
#include "SocketRunner.h"

namespace SL
{

class SOCKETLIB_API CTcpRunner : public CSocketRunner
{
public:
	CTcpRunner(void);
	virtual ~CTcpRunner(void);

	bool Open(unsigned short nConcurrencyThreadNum = 0, unsigned short nWorkThreadNum = 4);
	bool Close();
private:
	static unsigned int WINAPI Run(LPVOID lParam);

};

};
