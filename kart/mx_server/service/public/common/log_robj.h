/*
	file : gate_robj
	author : LargeWC
	created : 2020/04/17
*/

#ifndef _log_robj_
#define _log_robj_

//给一个默认密码，日志和工具使用的
#define ADMIN_PASSWORD 123456789

class CLog_RCObj : public UCVF_RemoteCallObj
{
public:
	//注册日志模块，bEnablePosWindows异常是否弹窗
	UCString AddModule(const UCString& strModName, const UCString& strServerName, int iPID, int& bEnablePosWindows)
	{
		return UCString("本地");
	}
};

#endif