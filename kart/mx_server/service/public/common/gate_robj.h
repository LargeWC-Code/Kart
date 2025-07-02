/*
	file : gate_robj
	author : LargeWC
	created : 2020/04/17
*/

#ifndef _gate_robj_
#define _gate_robj_

#include "public_base.h"

class CGate_RCObj : public UCVF_RemoteCallObj
{
	//登录第1步，获取一个账号服地址，附带版本校验和维护公告
	UCString	GetPlatformAccountUrlInfo(int iChannelID, UCString& AccoutUrl, int ClientVerNum, int& ServerVerNum)
	{
		return UCString("本地");
	}
};

#endif