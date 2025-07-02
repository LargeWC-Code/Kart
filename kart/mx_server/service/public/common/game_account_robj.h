#ifndef _account_
#define _account_

#include "public_base.h"

//账号登录服
class CGame_Account_RCObj : public UCVF_RemoteCallObj
{
public:
	//普通用户名密码登录，返回玩家UserID和随机key
	UCString Login(const UCString& Username, const UCString& Password, CUserGameID& UserGameID, int& AccountID, ucDWORD& AccountKey, UCString& GameDBUrl)
	{
		return UCString("本地");
	}
	//注册用户（用不到了，都走自动注册流程）
	UCString Register(const UCString& Username, const UCString& Password, CUserGameID& UserGameID, int& AccountID, ucDWORD& AccountKey, UCString& GameDBUrl)
	{
		return UCString("本地");
	}
	//验证key的合法性，给GameDB校验用户合法性，返回渠道ID和渠道扩展数据
	UCString CheckValid(const CUserGameID& UserGameID, ucDWORD AccountKey, int& iChannelID, UCString& strExtInfo, UCString& strUnionID)
	{
		return UCString("本地");
	}
};

#endif