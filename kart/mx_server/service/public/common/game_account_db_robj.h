#ifndef _account_db_
#define _account_db_

#include "public_base.h"

class CGame_Account_DB_RCObj : public UCVF_RemoteCallObj
{
public:
	//通过渠道ID，用户名，读取数据库，返回用户ID，扩展数据(密码)
	UCString LoadData(ucINT CanalID, ucCONST UCString& Username, UCString& ExtInfo, CUserGameID& UserGameID)
	{
		return UCString("本地");
	}
	//通过渠道ID，用户名，扩展数据(密码)，存储数据库，生成并返回用户ID
	UCString SaveData(ucINT CanalID, ucCONST UCString& Username, ucCONST UCString& ExtInfo, CUserGameID& UserGameID)
	{
		return UCString("本地");
	}
	//修改玩家用户名（也可以用来把匿名用户修改成正式用户）
	UCString ChaneUserName(int CanalID, const UCString& strOldUsername, const UCString& strNewUsername)
	{
		return UCString("本地");
	}
};

#endif