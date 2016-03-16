#ifndef __ICE_BASE_
#define __ICE_BASE_



#include "Ice.h"



#define ICY_ASSET_UNRET0(x)\
{\
	if (!(x)) \
	{\
		AfxMessageBox("asset fail");\
	}\
}

#define ICY_ASSET_RET(x, ret)\
{\
	if (!(x)) \
	{\
		AfxMessageBox("asset fail");\
        return (ret); \
	}\
}



#define ICY_ASSET_RET_LOG(x, log, ret)\
{\
	if (!(x)) \
	{\
		AfxMessageBox(log);\
        return (ret); \
	}\
}



#endif